//---------------------------------------------------------------------------
#ifndef A1730_BioDAQH
#define A1730_BioDAQH

#ifdef A1730_BioDAQR
#include<SyncObjs.hpp>
#include "c:/Advantech/BioDAQ/Inc/BDaqCL.h"

//---------------------------------------------------------------------------
class A1730_BioDAQ:protected TThread
{
private:
	BDaqDevice *device;
	BDaqDio *dio;
protected:

	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
	void WriteSignals(void);
	void ReadSignals(void);
	A1730_BioDAQ(int _DevNum);
	__fastcall ~A1730_BioDAQ(void);
};
//---------------------------------------------------------------------------
#endif
#endif
