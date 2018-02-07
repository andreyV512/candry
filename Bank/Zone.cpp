// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Zone.h"
#include "RawStrobe.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)
Zone::Zone(void)
{
	dead_begin_complete=false;
	dead_end_complete=false;
}
Zone::Zone(RawStrobe* _rawStrobe, int _zone_size, int _sensors,double _dead_level,double _thickness_noms)
{
	sensor.Capacity(_sensors);
	for (int i = 0; i < _sensors; i++)
		sensor.Add(new Sensor(_rawStrobe, _zone_size, i, _sensors,_dead_level,_thickness_noms));
	dead_begin_complete=false;
	dead_end_complete=false;
}

void Zone::Calc(CalcMeas* _calcMeas)
{
	thickness = 0;
	thickness_median = 0;
	bool first = true;
	for (int i = 0; i < sensor.Count(); i++)
	{
		sensor[i]->Calc(_calcMeas);
		if (first)
		{
			first = false;
			thickness = sensor[i]->thickness;
			thickness_median = sensor[i]->thickness_median;
		}
		else
		{
			if (thickness > sensor[i]->thickness)
				thickness = sensor[i]->thickness;
			if (thickness_median > sensor[i]->thickness_median)
				thickness_median = sensor[i]->thickness_median;
		}
	}
}

void Zone::Save(TFileStream* _fs)
{
	_fs->Write(&thickness, sizeof(thickness));
	_fs->Write(&thickness_median, sizeof(thickness_median));
	int n = sensor.Count();
	_fs->Write(&n, sizeof(n));
	for (int i = 0; i < sensor.Count(); i++)
		sensor[i]->Save(_fs);
}

void Zone::Load(TFileStream* _fs, Liner* _liner,double _dead_level,double _thickness_noms)
{
	_fs->Read(&thickness, sizeof(thickness));
	_fs->Read(&thickness_median, sizeof(thickness_median));
	int n;
	_fs->Read(&n, sizeof(n));
	sensor.Capacity(n);
	for (int i = 0; i < n; i++)
		sensor.Add()->Load(_fs, _liner,_dead_level,_thickness_noms);
}
double Zone::SetDeadBegin(double _portion,double _max)
{
	if(dead_begin_complete)
		return(_portion-1);
	dead_begin_complete=true;
	double ret;
	for (int i = 0; i < sensor.Count(); i++)
		ret=sensor[i]->SetDeadBegin(_portion,_max);
	return(ret);
}
double Zone::SetDeadEnd(double _portion,double _max)
{
	if(dead_end_complete)
		return(_portion-1);
	dead_end_complete=true;
	double ret;
	for (int i = 0; i < sensor.Count(); i++)
		ret=sensor[i]->SetDeadEnd(_portion,_max);
	return(ret);
}
void Zone::FillStats(std::map<th_status, uint32_t>* _stats)
{
	for (int i = 0; i < sensor.Count(); i++)
		sensor[i]->FillStats(_stats);
}
void Zone::SetDead(void)
{
	thickness = 0;
	thickness_median = 0;
	bool first = true;
	for (int i = 0; i < sensor.Count(); i++)
	{
		sensor[i]->SetDead();
		if (first)
		{
			first = false;
			thickness = sensor[i]->thickness;
			thickness_median = sensor[i]->thickness_median;
		}
		else
		{
			if (thickness > sensor[i]->thickness)
				thickness = sensor[i]->thickness;
			if (thickness_median > sensor[i]->thickness_median)
				thickness_median = sensor[i]->thickness_median;
		}
	}
}


