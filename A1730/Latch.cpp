//---------------------------------------------------------------------------
#pragma hdrstop
#include "Latch.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
Latch::Latch(bool _value,  CSignal* _signal)
{
	value=_value;
	signal=_signal;
	terminate=false;
	reason=AnsiString();
}


