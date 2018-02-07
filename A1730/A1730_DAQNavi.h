//---------------------------------------------------------------------------
#ifndef A1730_DAQNaviH
#define A1730_DAQNaviH
#ifdef A1730_DAQNaviR
//---------------------------------------------------------------------------
#pragma warn -8060
#include "c:/Advantech/DAQNavi/Inc/bdaqctrl.h"
#pragma warn .8060
#include<SyncObjs.hpp>
using namespace Automation::BDaq;

//---------------------------------------------------------------------------
class A1730_DAQNavi:protected TThread
{
private:
	InstantDiCtrl* instantDiCtrl;
	InstantDoCtrl* instantDoCtrl;
protected:

	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
	void WriteSignals(void);
	void ReadSignals(void);
	A1730_DAQNavi(int _DevNum);
	__fastcall ~A1730_DAQNavi(void);
};
//---------------------------------------------------------------------------
#endif
#endif

