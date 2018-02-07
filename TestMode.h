// ---------------------------------------------------------------------------
#ifndef TestModeH
#define TestModeH
// ---------------------------------------------------------------------------
#include<Classes.hpp>
#include "RawStrobes.h"
#include "CL10M8PCI_SDK2.h"

class TestMode : public TThread
{
private:
	int period;
	unsigned int startTick;
	RawStrobes* rawStrobes;
	int zone_size;
	void __fastcall Execute();
	void pr(AnsiString _msg);
	ADCBoards* adcBoards;
//	void ToFile(void);
public:

	TestMode(int _period, RawStrobes* _rawStrobes,ADCBoards* _adcBoards);
};
#endif
