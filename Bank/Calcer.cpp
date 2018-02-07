// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Calcer.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

Calcer::Calcer(RawStrobes* _rawStrobes, unsigned int _period, bool _RagACF,
	RThickness::MathSettings _msettings,TIniFile* _ini)
{
	calcMeas=new CalcMeas(_RagACF,_msettings,_ini);
	period = _period;
	rawStrobes = _rawStrobes;
}
__fastcall Calcer::~Calcer(void)
{
	delete calcMeas;
}

void __fastcall Calcer::Execute()
{
	RawStrobe* rawStrobe;
	for (; ;)
	{
		if (Terminated)
			break;
		rawStrobe = rawStrobes->GetCalc();
		if (rawStrobe != NULL)
		{
			for (int i = 0; i < rawStrobe->sensors; i++)
				calcMeas->Exec((rawStrobe->meas) + i);
			rawStrobes->FreeCalc(rawStrobe);
			if (Terminated)
				break;
		}
		else
			Sleep(period);
	}
}

