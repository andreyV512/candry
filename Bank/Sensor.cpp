// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Sensor.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

Sensor::Sensor(RawStrobe* _rawStrobe, int _zone_size, int _index, int _sensors,double _dead_level,double _thickness_noms)
{
	index = _index;
	meas.Capacity(_zone_size);
	RawStrobe* pp = _rawStrobe + _zone_size;
	for (; _rawStrobe < pp; _rawStrobe++)
		meas.Add(_rawStrobe->meas + index);
	dead_level=_dead_level;
	thickness_noms=_thickness_noms;
}

void Sensor::Calc(CalcMeas* _calcMeas)
{
	double* m0 = new double[meas.Count()];
	double* m1 = new double[meas.Count()];
	for (int i = 0; i < meas.Count(); i++)
	{
		if (_calcMeas != NULL)
			_calcMeas->Exec(meas[i]);
		m0[i] = meas[i]->thickness;
	}
	medianfilter(m0, m1, meas.Count());
	for (int i = 0; i < meas.Count(); i++)
		meas[i]->thickness_median = m1[i];
	delete[]m0;
	delete[]m1;
	thickness = dead_level;
	thickness_median = dead_level;
	bool first = true;
	for (int i = 0; i < meas.Count(); i++)
	{
		if (first)
		{
			first = false;
			thickness = meas[i]->thickness;
			thickness_median = meas[i]->thickness_median;
		}
		else
		{
			if (thickness > meas[i]->thickness)
				thickness = meas[i]->thickness;
			if (thickness_median > meas[i]->thickness_median)
				thickness_median = meas[i]->thickness_median;
		}
	}
	if(thickness_noms!=0)
	{
		if(	thickness == dead_level)
			thickness=thickness_noms;
		if(	thickness_median == dead_level)
			thickness_median=thickness_noms;
	}
}

void Sensor::Save(TFileStream* _fs)
{
	_fs->Write(&thickness, sizeof(thickness));
	_fs->Write(&thickness_median, sizeof(thickness_median));
	_fs->Write(&index, sizeof(index));
	int n = meas.Count();
	_fs->Write(&n, sizeof(n));
	for (int i = 0; i < meas.Count(); i++)
		meas[i]->Save(_fs);
}

void Sensor::Load(TFileStream* _fs, Liner* _liner,double _dead_level,double _thickness_noms)
{
	thickness_noms=_thickness_noms;
	dead_level=_dead_level;
	_fs->Read(&thickness, sizeof(thickness));
	_fs->Read(&thickness_median, sizeof(thickness_median));
	_fs->Read(&index, sizeof(index));
	int n;
	_fs->Read(&n, sizeof(n));
	meas.Capacity(n);
	for (int i = 0; i < n; i++)
		meas.Add(_liner->Get())->Load(_fs);
}

double Sensor::SetDeadBegin(double _portion, double _max)
{
	double nd = meas.Count();
	int n = (int)(nd * _portion);
	if (n > meas.Count())
		n = meas.Count();
	for (int i = 0; i < n; i++)
		meas[i]->thickness = _max;
	return(_portion-1);
}

double Sensor::SetDeadEnd(double _portion, double _max)
{
	double nd = meas.Count();
	int n = meas.Count() - (int)(nd * _portion);
	if (n < 0)
		n = 0;
	for (; n < meas.Count(); n++)
		meas[n]->thickness = _max;
	return(_portion-1);
}
void Sensor::FillStats(std::map<th_status, uint32_t>* _stats)
{
	for (int i=0; i < meas.Count(); i++)
		(*_stats)[meas[i]->thCode]++;
}
void Sensor::SetDead(void)
{
		thickness = dead_level;
		thickness_median = dead_level;
}


