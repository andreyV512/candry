// ---------------------------------------------------------------------------
#ifndef RawStrobeH
#define RawStrobeH
#include "Meas.h"

// ---------------------------------------------------------------------------
class RawStrobe
{
private:
	Meas* p_max;
	int meas_size;
public:
	int sensors;
	bool calced;
	Meas* meas;
	unsigned int tick;

	void Init(int _meas_size,int _sensors);
	~RawStrobe(void);
	char* Fill(char* _data, unsigned int _tick);
	char* Fill(char* _data, unsigned int _tick,int _single_sensor);
	char* Zero(char* _data);
	void ToFile(AnsiString _fname);
};
#endif
