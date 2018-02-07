// ---------------------------------------------------------------------------

#ifndef ZoneH
#define ZoneH
#include "Sensor.h"
#include "uCRListT.h"
#include "RawStrobe.h"
#include "CalcMeas.h"
// ---------------------------------------------------------------------------
class Zone
{
private:
	bool dead_begin_complete;
	bool dead_end_complete;
public:
	double thickness;
	double thickness_median;
	CRListT<Sensor>sensor;
	int nn;

	Zone(void);
	Zone(RawStrobe* _rawStrobe, int _zone_size, int _sensors,double _dead_level,double _thickness_noms);
	void Calc(CalcMeas* _calcMeas=NULL);
	void Save(TFileStream* _fs);
	void Load(TFileStream* _fs,Liner* _liner,double _dead_level,double _thickness_noms);
	double SetDeadBegin(double _portion,double _max);
	double SetDeadEnd(double _portion,double _max);
	void FillStats(std::map<th_status, uint32_t>* _stats);
	void SetDead(void);
};
#endif
