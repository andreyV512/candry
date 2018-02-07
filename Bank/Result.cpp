// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Result.h"
#include <IOUtils.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

/*
 Result::Result(void)
 {
 dead_level = 0;
 portion_begin = 0;
 portion_end = 0;
 sensors = 0;
 zone_wait=0;
 zone_given=0;
 }
 */

Result::Result(int _dead_begin, int _dead_end, int _zone_size,
	double _dead_level, int _sensors,double _thickness_noms)
{
	dead_level = _dead_level;
	thickness_noms=_thickness_noms;
	portion_begin = _dead_begin;
	portion_begin /= _zone_size;
	portion_end = _dead_end;
	portion_end /= _zone_size;
	sensors = _sensors;
	zone_wait = _dead_end / _zone_size;
	if (zone_wait*_zone_size < _dead_end)
		zone_wait++;
	zone_given = 0;
	dead_zones_begin=_dead_begin/_zone_size;
	dead_zones_end=_dead_end/_zone_size;
}

void Result::Calc()
{
	for (int i = 0; i < this->zone.Count(); i++)
		zone[i]->Calc();
}

void Result::ReCalc(bool _RagACF, RThickness::MathSettings _msettings,TIniFile* _ini)
{
	CalcMeas* calcMeas = new CalcMeas(_RagACF, _msettings,_ini);
	for (int i = 0; i < zone.Count(); i++)
	{
		zone[i]->Calc(calcMeas);
		if(i<dead_zones_begin)
			zone[i]->SetDead();
		if(i>=zone.Count()-dead_zones_end)
			zone[i]->SetDead();
	}
	delete calcMeas;
}

int Result::SensorCount(void)
{
	if (zone.Count() <= 0)
		return (0);
	else
		return (zone[0]->sensor.Count());

}

CRListLT<Meas> * Result::evalZone(int _SensorNo, int _ZoneNo)
{
	return (&zone[_ZoneNo]->sensor[_SensorNo]->meas);
}

void Result::Clear(void)
{
	zone.Clear();
	last = false;
	zone_given = 0;
}

void Result::Save(TFileStream* _fs)
{
	int n = zone.Count();
	_fs->Write(&n, sizeof(n));
	for (int i = 0; i < zone.Count(); i++)
		zone[i]->Save(_fs);
}

void Result::Load(TFileStream* _fs, Liner* _liner)
{
	int n;
	_fs->Read(&n, sizeof(n));
	zone.Capacity(n);
	for (int i = 0; i < n; i++)
		zone.Add()->Load(_fs, _liner,dead_level,thickness_noms);
}

void Result::SetDeads(bool _finish)
{
	double portion;
	portion = portion_begin;
	for (int i = 0; i < zone.Count(); i++)
	{
		if (portion <= 0)
			break;
		portion = zone[i]->SetDeadBegin(portion, dead_level);
	}
	if (!_finish)
		return;
	portion = portion_end;
	for (int i = zone.Count() - 1; i >= 0; i--)
	{
		if (portion <= 0)
			break;
		portion = zone[i]->SetDeadEnd(portion, dead_level);
	}
}

void Result::AddZone(RawStrobe* _rawStrobe, int _zone_size, bool _last)
{
	if (_rawStrobe == NULL)
	{
		if (_last)
		{
			last = true;
			SetDeads(_last);
		}
	}
	else
	{
		Zone* z = zone.Add(new Zone(_rawStrobe, _zone_size, sensors,dead_level,thickness_noms));
		z->nn=zone.Count()-1;
		SetDeads(_last);
		if (_last)
			last = true;
	}
}

Zone* Result::GetNewZone(bool* _last)
{
	if (zone_given >= zone.Count())
	{
		*_last = last;
		return (NULL);
	}
	if (last)
	{
		Zone* z = zone[zone_given];
		z->Calc();
		if(zone_given>=zone.Count()-dead_zones_end)
			z->SetDead();
		zone_given++;
		*_last=zone_given == zone.Count();
		return (z);
	}
	if (zone_given + zone_wait >= zone.Count())
	{
		*_last = false;
		return (NULL);
	}
	Zone* z = zone[zone_given];
	z->Calc();
	if(zone_given<dead_zones_begin)
		z->SetDead();
	zone_given++;
	*_last = false;
	return (z);
}
std::map<th_status, uint32_t> Result::GetStats(void)
{
	std::map< th_status, uint32_t >	stats;
	for (int i = 0; i < zone.Count(); i++)
		zone[i]->FillStats(&stats);
	return(stats);
}
AnsiString Result::StatusString(th_status _thCode)
{
	if (_thCode == TH_OK)
		return ("Рассчитано");
	if (_thCode == TH_EXTREMUMS_NOT_FOUND)
		return ("Пики не найдены");
	if (_thCode == TH_GREATER_THAN_MAX)
		return ("Первый пик дальше верхней границы");
	if (_thCode == TH_LESS_THAN_MIN)
		return ("Первый пик ближе нижней границы");
	if (_thCode == TH_NOT_ENOUGH_INFO)
		return ("Недостаточно данных для рассчета");
	if (_thCode == TH_BIG_DIFFERENCE)
		return ("Разница между пиками больше погрешности");
	if (_thCode == TH_SMALL_MAXIMUM)
		return ("Отношение максимума к минимуму плохое");
	if (_thCode == TH_BIG_ENERGY)
		return ("Энергия больше верхнего порога");
	if (_thCode == TH_SMALL_ENERGY)
		return ("Энергия меньше нижнего порога");
	if (_thCode == TH_MAXIMUM_TO_ENERGY)
		return ("Отношение максимума к энергии ниже требуемого");
	if (_thCode == TH_MAXIMUM_PEAKS)
		return ("Количество пиков слишком велико");
	if (_thCode == TH_DJ)
		return ("Недостаточен индекс DJ");
	return ("Неопределено");
}


