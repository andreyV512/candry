// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Liner.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

Liner::Liner(RawStrobe* _rawStrobe, int _size)
{
	rawStrobe = _rawStrobe;
	size = _size;
	index = 0;
	index_sensor = 0;
}

Meas* Liner::Get(void)
{
	if (index_sensor >= rawStrobe->sensors)
	{
		index++;
		if (index >= size)
			throw(Exception("Liner::Get: выход за пределы"));
		index_sensor = 0;
	}
	Meas* ret=(rawStrobe+index)->meas + index_sensor;
	index_sensor++;
	return (ret);
}
