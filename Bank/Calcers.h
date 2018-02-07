//---------------------------------------------------------------------------
#ifndef CalcersH
#define CalcersH
#include "uCRListT.h"
#include "Calcer.h"
//---------------------------------------------------------------------------
class Calcers
{
private:
	CRListT<Calcer> items;
	int count;
	int period;
	bool RagACF;
	RawStrobes* rawStrobes;
public:
	Calcers(int _count,int _period,RawStrobes* _rawStrobes,bool _RagACF, RThickness::MathSettings _msettings,TIniFile* _ini);
	void ReStart(RThickness::MathSettings _msettings,TIniFile* _ini);
};
#endif
