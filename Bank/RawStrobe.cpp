// ---------------------------------------------------------------------------
#pragma hdrstop
#include "RawStrobe.h"
#include "stdio.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

void RawStrobe::Init(int _meas_size,int _sensors)
{
	sensors=_sensors;
	meas = new Meas[_sensors];
	p_max = meas + _sensors;
	for(int i=0;i<sensors;i++)
		meas[i].Init(_meas_size,i);
	calced=false;
	meas_size=_meas_size;
}
RawStrobe::~RawStrobe(void)
{
	delete[] meas;
}

char* RawStrobe::Fill(char* _data,unsigned int _tick)
{
	for (Meas * p = meas; p < p_max; p++)
		_data = p->Fill(_data);
	tick=_tick;
	calced=false;
	return (_data);
}
char* RawStrobe::Fill(char* _data,unsigned int _tick,int _single_sensor)
{
	char* data_single=_data+meas_size*_single_sensor;
	for (Meas * p = meas; p < p_max; p++)
		p->Fill(data_single);
	_data+=meas_size*sensors;
	tick=_tick;
	calced=false;
	return (_data);
}
void RawStrobe::ToFile(AnsiString _fname)
{
		FILE *df = fopen(_fname.c_str(), "w");
		if (df==NULL)
			return;
		for(int i=0;i<sensors;i++)
		{
			for(int j=0;j<meas_size;j++)
				fprintf(df,"[%d][%d]=%d\n",i,j,(int)meas[i].data[j]);
		}
		fclose(df);
}

