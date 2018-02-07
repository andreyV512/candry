// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Calcers.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

Calcers::Calcers(int _count, int _period, RawStrobes* _rawStrobes, bool _RagACF,
	RThickness::MathSettings _msettings,TIniFile* _ini)
{
	rawStrobes = _rawStrobes;
	count = _count;
	period = _period;
	RagACF = _RagACF;
	for (int i = 0; i < count; i++)
		items.Add(new Calcer(rawStrobes, period, RagACF, _msettings,_ini));
}

void Calcers::ReStart(RThickness::MathSettings _msettings,TIniFile* _ini)
{
	items.Clear();
	for (int i = 0; i < count; i++)
		items.Add(new Calcer(rawStrobes, period, RagACF, _msettings,_ini));
}
