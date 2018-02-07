#ifndef CL10M8PCI_SDK2H
#define CL10M8PCI_SDK2H
#pragma warn -8008
#pragma warn -8012
#pragma warn -8058
#pragma warn -8066
#pragma warn -8071
#pragma warn -8072
#include "RshApi.cpp"
#pragma warn .8008
#pragma warn .8012
#pragma warn .8058
#pragma warn .8066
#pragma warn .8071
#pragma warn .8072
#include <System.hpp>
#include <Classes.hpp>
#include "AcquireSettings.h"
#include <stdio.h>

// ---------------------------------------------------------------------------
class CL10M8PCI_SDK2
{
public:
	typedef void (__closure * OnPrDef)(AnsiString _msg);
private:
	CL10M8PCI_SDK2(int _baseAddress,OnPrDef _OnPr);
	bool error;
	IRshDevice* device;
	RshDllClient* Client;
	void pr(AnsiString _msg);
	AnsiString GetErr(unsigned int _error);
	// Буфер для данных (в МЗР)
	RSH_BUFFER_S8 data;
	HANDLE hIRQEvent;
	void __fastcall Execute();
	int baseAddress;
//	char gbuf[100*3*484];
//	char* pgbuf;
//	int ibuf;
public:
	static CL10M8PCI_SDK2* Create(int _baseAddress,OnPrDef _OnPr=NULL);
	__fastcall ~CL10M8PCI_SDK2(void);
	bool initSettings(AcquireSettings* _settings);
	inline GetbaseAddress(void)
	{
		return (baseAddress);
	}
	bool Read(char* _buf);
	int GetBufSize(void);
	OnPrDef OnPr;
//	void ToFile(FILE* df);
};
// ---------------------------------------------------------------------------
class ADCBoards
{
public:
	typedef void (__closure * OnPrDef)(AnsiString _msg);
private:
	ADCBoards(int _baseAddress0,int _baseAddress1,OnPrDef _OnPr);
	bool error;
	OnPrDef OnPr;
public:
	CL10M8PCI_SDK2* board0;
	CL10M8PCI_SDK2* board1;
	static ADCBoards* Create(int _baseAddress0,int _baseAddress1,OnPrDef _OnPr=NULL);
	__fastcall ~ADCBoards(void);
	bool initSettings(AcquireSettings* _settings);
	int GetBufSize(void);
	void SetPr(OnPrDef _OnPr);
};
// ---------------------------------------------------------------------------
#endif
