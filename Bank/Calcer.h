// ---------------------------------------------------------------------------
#ifndef CalcerH
#define CalcerH
// ---------------------------------------------------------------------------
#include "RawStrobes.h"
#include "CalcMeas.h"

class Calcer : private TThread
{
private:

	void __fastcall Execute();

	unsigned int period;
	RawStrobes* rawStrobes;
	CalcMeas* calcMeas;
public:
	Calcer(RawStrobes* _rawStrobes, unsigned int _period, bool _RagACF,
		RThickness::MathSettings _msettings,TIniFile* _ini);
	__fastcall ~Calcer(void);
};
#endif
