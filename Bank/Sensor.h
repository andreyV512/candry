// ---------------------------------------------------------------------------

#ifndef SensorH
#define SensorH
#include "uCRListLT.h"
#include "RawStrobe.h"
#include "CalcMeas.h"
#include "Liner.h"
// ---------------------------------------------------------------------------
class Sensor
{
private:
	double dead_level;
	double thickness_noms;
//	void Calc(CalcMeas* _calcMeas=NULL);
public:
	double thickness;
	double thickness_median;
	int index;

	CRListLT<Meas>meas;
	Sensor(RawStrobe* _rawStrobe, int _zone_size, int _index, int _sensors,double _dead_level,double _thickness_noms);
	Sensor(void){};
	void Calc(CalcMeas* _calcMeas=NULL);
	void Save(TFileStream* _fs);
	void Load(TFileStream* _fs, Liner* _liner,double _dead_level,double _thickness_noms);
	double SetDeadBegin(double _portion,double _max);
	double SetDeadEnd(double _portion,double _max);
	void FillStats(std::map<th_status, uint32_t>* _stats);
	void SetDead(void);
};
#endif
