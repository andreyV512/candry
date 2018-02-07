// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Meas.h"
#include<locale.h>

// ---------------------------------------------------------------------------

#pragma package(smart_init)

void Meas::Init(int _size, int _n_sensor)
{
	thickness = 0;
	thickness_median = 0;
	size = _size;
	size_acf = _size;
	data = new unsigned char[size];
	acf = new double[size];
	n_sensor = _n_sensor;
	dj=-1;
}

Meas::~Meas(void)
{
	delete[]data;
	delete[]acf;
}

char* Meas::Fill(char* _data)
{
	char* p= data;
	char* pp = _data + size;
	for (; _data < pp;_data++,p++)
	{
		* p = (*_data);
	}
	return (_data);
}

void Meas::Save(TFileStream* _fs)
{
	_fs->Write(&thickness, sizeof(thickness));
	_fs->Write(&thickness_median, sizeof(thickness_median));
	_fs->Write(&n_sensor, sizeof(n_sensor));
	_fs->Write(&thCode, sizeof(thCode));
	_fs->Write(&size, sizeof(size));
	_fs->Write(data, sizeof(*data)*size);
	_fs->Write(&size_acf, sizeof(size_acf));
	_fs->Write(acf, sizeof(*acf)*size_acf);
	int n=acfPeaks.size();
	_fs->Write(&n, sizeof(n));
	for(int i=0;i<n;i++)
	{
		RThickness::CurvePeak cp=acfPeaks[i];
		_fs->Write(&(cp), sizeof(cp));
	}
}
void Meas::Load(TFileStream* _fs)
{
	_fs->Read(&thickness, sizeof(thickness));
	_fs->Read(&thickness_median, sizeof(thickness_median));
	_fs->Read(&n_sensor, sizeof(n_sensor));
	_fs->Read(&thCode, sizeof(thCode));
	_fs->Read(&size, sizeof(size));
	_fs->Read(data, sizeof(*data)*size);
	_fs->Read(&size_acf, sizeof(size_acf));
	_fs->Read(acf, sizeof(*acf)*size_acf);
	int n;
	_fs->Read(&n, sizeof(n));
	acfPeaks.resize(n);
	for(int i=0;i<n;i++)
		_fs->Read(&(acfPeaks[i]), sizeof(acfPeaks[i]));
}

