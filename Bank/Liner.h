// ---------------------------------------------------------------------------
#ifndef LinerH
#define LinerH
#include "RawStrobe.h"

// ---------------------------------------------------------------------------
class Liner
{
private:
	RawStrobe* rawStrobe;
	int size;
	int index;
	int index_sensor;
public:
	Liner(RawStrobe* _rawStrobe,int _size);
	Meas* Get(void);
};
#endif
