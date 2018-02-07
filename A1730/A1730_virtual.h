//---------------------------------------------------------------------------
#ifndef A1730_virtualH
#define A1730_virtualH
//---------------------------------------------------------------------------
#include<SyncObjs.hpp>
//---------------------------------------------------------------------------
class A1730_virtual:protected TThread
{
protected:
	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
	void WriteSignals(void);
	void ReadSignals(void);
	A1730_virtual(int _DevNum);
	__fastcall ~A1730_virtual(void);
};
//---------------------------------------------------------------------------
#endif
