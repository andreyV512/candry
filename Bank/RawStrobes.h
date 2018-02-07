// ---------------------------------------------------------------------------

#ifndef RawStrobesH
#define RawStrobesH
#include "RawStrobe.h"
#include "uCRListT.h"
#include "TickStrobe.h"
#include "Result.h"
// ---------------------------------------------------------------------------
class RawStrobes
{
private:
	int max_packets;
	int packet_size;
	int sensors;
	int meas_size;
	TCriticalSection* cs;
	double strobe_period;
	RawStrobe* items;
	RawStrobe* p_max;
	unsigned int packet_period;

	RawStrobe* p_zoned;
	RawStrobe* p_added;
	RawStrobe* p_calcing;
	RawStrobe* p_calced;
	unsigned int start_tick;
	unsigned int stop_tick;

	CRListT<TickStrobe>ticks;
	bool complete;
	bool complete2;
	RawStrobe* GetNewZone0(int* _zone_size);
	unsigned int waitPeriod;
	int rawStrobeSize;
	int count_zones;
	Result* result;
	inline void pr(AnsiString _msg);
	int wait_period;
	int test_ticks;
	double dead_level;
public:
	RawStrobes(int _max_packets, int _packet_size, int _sensors, int _meas_size,
		unsigned int _MeasureFrequency_hz,int _wait_period,int _dead_begin, int _dead_end, int _zone_size,
	double _dead_level,double _thickness_noms);
	~RawStrobes(void);
	void Clear(void);
	void StartTick(unsigned int _tick);
	void StopTick(unsigned int _tick);
	void AddTickStrobe(unsigned int _tick);
	void Add(char* _data, unsigned int _tick);
	RawStrobe* GetCalc(void);
	void FreeCalc(RawStrobe* _rawStrobe);
	Zone* GetNewZone(void);
	Zone* GetNewZoneTest(unsigned int _size);
	Result* GetResult(void);
	bool IsComplete(void);
	void SetTestComplete(void);
	bool LoadResult(AnsiString _fname);
	bool SaveResult(AnsiString _fname);
	int single_sensor;
};
#endif
