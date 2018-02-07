// ---------------------------------------------------------------------------
#pragma hdrstop
#include "RawStrobes.h"
#include <System.hpp>
#include <SysUtils.hpp>
#include "Protocol.h"
#include <IOUtils.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

RawStrobes::RawStrobes(int _max_packets, int _packet_size, int _sensors,
	int _meas_size, unsigned int _MeasureFrequency_hz, int _wait_period,int _dead_begin, int _dead_end, int _zone_size,
	double _dead_level,double _thickness_noms)
{
	dead_level=_dead_level;
	max_packets = _max_packets;
	packet_size = _packet_size;
	sensors = _sensors;
	meas_size = _meas_size;
	rawStrobeSize = meas_size * sensors;
	wait_period = _wait_period;

	double packet_periodD = 1000;
	packet_periodD *= _packet_size;
	packet_periodD /= _MeasureFrequency_hz;
	packet_period = (unsigned int)packet_periodD;
	strobe_period = packet_period;
	strobe_period /= _packet_size;
	items = new RawStrobe[_max_packets * _packet_size];
	p_max = items + _max_packets * _packet_size;
	for (RawStrobe * p = items; p < p_max; p++)
		p->Init(meas_size, _sensors);
	cs = new TCriticalSection;
	result=new Result(_dead_begin,_dead_end,_zone_size,_dead_level,sensors,_thickness_noms);
	Clear();
	single_sensor=-1;
}

RawStrobes::~RawStrobes(void)
{
	delete[]items;
	delete cs;
	delete result;
}

void RawStrobes::Clear(void)
{
	cs->Enter();
	{
		p_added = items;
		p_calcing = items;
		p_calced = items;
		p_zoned = items;
		start_tick = 0;
		stop_tick = 0;
		ticks.Clear();
		complete = false;
		complete2 = false;
		count_zones = 0;
		result->Clear();
	} cs->Leave();
	pr("RS: Clear");
}

void RawStrobes::StartTick(unsigned int _tick)
{
	AnsiString a = "PR: StartTick=";
	cs->Enter();
	{
		if (start_tick == 0)
		{
			start_tick = _tick;
			a += _tick;
			test_ticks=0;
		}
	} cs->Leave();
	pr(a);
}

void RawStrobes::StopTick(unsigned int _tick)
{
	AnsiString a = "RS: StopTick=";
	cs->Enter();
	{
		if (stop_tick == 0)
		{
			if (start_tick == 0)
				complete = true;
			else
			{
				stop_tick = _tick;
				ticks.Add(new TickStrobe(_tick));
				a += _tick - start_tick;
			}
		}
	} cs->Leave();
//	pr(a);
}

void RawStrobes::AddTickStrobe(unsigned int _tick)
{
	AnsiString a = "RS: AddTickStrobe!=";
	cs->Enter();
	{
		if (start_tick != 0)
		{
			if (stop_tick == 0)
			{
				ticks.Add(new TickStrobe(_tick));
				a += _tick - start_tick;
				a+=" [";
				a+=++test_ticks;
				a+="]";
			}
		}
	} cs->Leave();
	pr(a);
}

void RawStrobes::Add(char* _data, unsigned int _tick)
{
	AnsiString a = "RS: Add=";
	char * data0=_data;
	cs->Enter();
	{
		if (complete)
		{
			cs->Leave();
			return;
		}
		if (start_tick == 0)
		{
			cs->Leave();
			return;
		}
		if (_tick <= start_tick)
		{
			cs->Leave();
			return;
		}
		if (p_added + packet_size > p_max)
		{
			AnsiString a = "RawStrobes::Add: Переполнение: пакет: ";
			a += (p_added + packet_size - items) / packet_size;
			throw(Exception(a));
		}
		double dtick = 0;
		unsigned int tick0 = _tick - packet_period;
		unsigned int cur_tick;
		RawStrobe* p = p_added;
		for (int i = 0; i < packet_size; i++)
		{
			cur_tick = tick0 + ((unsigned int)dtick);
			dtick += strobe_period;
			if (cur_tick <= start_tick)
			{
				_data += rawStrobeSize;
			}
			else
			{
				if(single_sensor<0)
					_data = p->Fill(_data, cur_tick);
				else
					_data = p->Fill(_data, cur_tick,single_sensor);
				p++;
			}
		}
		p_added = p;
		a += _tick - start_tick;
		a += " <";
		a += tick0 - start_tick;
		a += ":";
		a += cur_tick - start_tick;
		a += ">";
		a+=" ps=";
		a+=packet_size;
		a+=" dd=";
		a+=_data-data0;
	} cs->Leave();
	pr(a);
}

RawStrobe* RawStrobes::GetCalc(void)
{
	RawStrobe* ret;
	cs->Enter();
	{
		if (complete)
		{
			cs->Leave();
			return (NULL);
		}
		if (p_calcing >= p_added)
		{
			cs->Leave();
			return (NULL);
		}
		ret = p_calcing;
		p_calcing++;
	} cs->Leave();
	return (ret);
}

void RawStrobes::FreeCalc(RawStrobe * _rawStrobe)
{
	cs->Enter();
	{
		_rawStrobe->calced = true;
		for (; p_calced < p_added; p_calced++)
		{
			if (!p_calced->calced)
				break;
		}
	} cs->Leave();
}

Zone* RawStrobes::GetNewZone(void)
{
	int zone_size = 0;
	RawStrobe* ret = NULL;
	bool lcomplete;
	cs->Enter();
	{
		if (!complete)
			ret = RawStrobes::GetNewZone0(&zone_size);
		lcomplete = complete;
	} cs->Leave();
	result->AddZone(ret, zone_size, lcomplete);
	if (ret != NULL||lcomplete)
	{
		AnsiString a = "RS: GetNewZone: [";
		a += result->zone.Count()-1;
		a += "]=";
		a += zone_size;
		pr(a);
	}
	bool last;
	Zone* z = result->GetNewZone(&last);
	if (last)
		complete2 = true;
	if(z!=NULL)
	{
		AnsiString a = "RS: GetNewZone: [";
		a += z->nn;
		a += "] На гора";
		pr(a);
	}
	return (z);
}

RawStrobe* RawStrobes::GetNewZone0(int* _zone_size)
{
	*_zone_size = 0;
	RawStrobe* ret = NULL;
	bool finish = false;
	if (start_tick == 0)
		return (ret);
	if (stop_tick != 0)
	{
		if (p_calced != items)
			finish = ((p_calced - 1)->tick + wait_period) < GetTickCount();
		else
			finish = (start_tick + wait_period) < GetTickCount();
	}
	if (finish)
	{
		unsigned int tick = ticks[0]->tick;
		if (p_calced == items)
		{
			complete = true;
			return (ret);
		}
		if ((p_calced - 1)->tick <= tick)
		{
			ret = p_zoned;
			for (; p_zoned < p_calced; p_zoned++)
				(*_zone_size)++;
			complete = true;
		}
		else
		{
			ret = p_zoned;
			for (; p_zoned < p_calced; p_zoned++)
			{
				if (p_zoned->tick > tick)
					break;
				(*_zone_size)++;
			}
			ticks.Delete(0);
			complete = ticks.Count() == 0;
		}
		return (ret);
	}
	if (ticks.Count() == 0)
		return (ret);
	if (p_calced == items)
		return (ret);
	unsigned int tick = ticks[0]->tick;
	if ((p_calced - 1)->tick <= tick)
		return (ret);
	if (p_zoned >= p_calced)
		return (ret);
	ret = p_zoned;
	for (; p_zoned < p_calced; p_zoned++)
	{
		if (p_zoned->tick > tick)
			break;
		(*_zone_size)++;
	}
	ticks.Delete(0);
	if (stop_tick)
	{
		if (ticks.Count() == 0)
			complete = true;
	}
	return (ret);
}

Zone* RawStrobes::GetNewZoneTest(unsigned int _size)
{
	RawStrobe* ret;
	cs->Enter();
	{
		if (p_calced < (p_zoned + _size))
			ret = NULL;
		else
		{
			ret = p_zoned;
			p_zoned += _size;
			if (p_zoned > p_max)
				throw(Exception
				("RawStrobes::GetNewZoneTest: выход за пределы"));
		}
	} cs->Leave();
	if (ret == NULL)
		return (NULL);
	else
	{
		AnsiString a = "RS: GetNewZoneTest: ";
		a += result->zone.Count() + 1;
		pr(a);
		Zone* z=result->zone.Add(new Zone(ret, _size, sensors,dead_level,0));
		z->Calc();
		return (z);
	}
}

void RawStrobes::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}

Result* RawStrobes::GetResult(void)
{
	Result* ret = NULL;
	cs->Enter();
	{
		if (complete2)
			ret = result;
	} cs->Leave();
	return (ret);
}

bool RawStrobes::IsComplete(void)
{
	int ttt[5];
	bool ret;
	cs->Enter();
	{
		ret=complete2;
	} cs->Leave();
	return (ret);
}

void RawStrobes::SetTestComplete(void)
{
	cs->Enter();
	{
		complete = true;
		complete2 = true;
	} cs->Leave();
}

bool RawStrobes::SaveResult(AnsiString _fname)
{
	bool ret = false;
	cs->Enter();
	{
		if (complete)
		{
			try
			{
				if (TFile::Exists(_fname))
					TFile::Delete(_fname);
				TFileStream* fs = TFile::Create(_fname);
				result->Save(fs);
				fs->Free();
			}
			catch (...)
			{
				ret = false;
			}
			ret = true;
		}
	} cs->Leave();
	return (ret);
}

bool RawStrobes::LoadResult(AnsiString _fname)
{
	bool ret = false;
	cs->Enter();
	{
		Clear();
		try
		{
			TFileStream* fs = TFile::OpenRead(_fname);
			result->Clear();
			Liner* liner = new Liner(items, max_packets * packet_size);
			result->Load(fs, liner);
			delete liner;
			fs->Free();
		}
		catch (...)
		{
			result->Clear();
			ret = false;
		}
		ret = true;
		complete = true;
		complete2 = true;
	} cs->Leave();
	return (ret);
}
