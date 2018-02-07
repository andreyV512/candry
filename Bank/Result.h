// ---------------------------------------------------------------------------
#ifndef ResultH
#define ResultH
#include "Zone.h"

// ---------------------------------------------------------------------------
class Result
{
private:
	double dead_level;
	double thickness_noms;
	double portion_begin;
	double portion_end;
	void SetDeads(bool _finish);
	int sensors;
	bool last;
	int zone_wait;
	int zone_given;
	int dead_zones_begin;
	int dead_zones_end;

public:
//	Result(void);
	Result(	int _dead_begin,int _dead_end,int _zone_size,double _dead_level,int _sensors,double _thickness_noms);
	CRListT<Zone>zone;
	void Calc(void);
	void ReCalc(bool _RagACF, RThickness::MathSettings _msettings,TIniFile* _ini);
	int SensorCount(void);
	CRListLT<Meas> * evalZone(int _SensorNo, int _ZoneNo);
	void Clear(void);
	void Save(TFileStream* _fs);
	void Load(TFileStream* _fs, Liner* _liner);
	void AddZone(RawStrobe* _rawStrobe,int _zone_size,bool _last);
	Zone* GetNewZone(bool* _last);
	std::map<th_status, uint32_t> GetStats(void);
	AnsiString StatusString(th_status _thCode);
};
#endif
