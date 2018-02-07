// ---------------------------------------------------------------------------
#pragma hdrstop
#include "A1730_BioDAQ.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#ifdef A1730_BioDAQR
// ---------------------------------------------------------------------------
A1730_BioDAQ::A1730_BioDAQ(int _DevNum) : TThread(false)
{
	ErrorCode errorCode;
	device = NULL;
	errorCode = BDaqDevice::Open(_DevNum, ModeWrite, device);
#pragma warn -8012
	if (BioFailed(errorCode))
		throw(Exception("A1730_BioDAQ::A1730_BioDAQ: не смогли открыть плату Advantech1730"));
#pragma warn .8012
	errorCode = device->GetModule(0, dio); // Get Dio Module
	if (errorCode == Success && ReadOnly)
		throw(Exception("A1730_BioDAQ::A1730_BioDAQ: не смогли открыть плату Advantech1730"));
}

// ---------------------------------------------------------------------------
__fastcall A1730_BioDAQ::~A1730_DAQNavi(void)
{
	device->Close();
}

// ---------------------------------------------------------------------------
DWORD A1730_BioDAQ::Read(void)
{
	BYTE buf[4];
	ErrorCode errorcode = dio->DiRead(startPort, portCount, buf);
	if (dio->DiRead(startPort, portCount, buf) != Success)
		throw(Exception("A1730_BioDAQ::Read: не могу прочитать плату"));
	DWORD v = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (v);
}

// ---------------------------------------------------------------------------
DWORD A1730_BioDAQ::ReadOut(void)
{
	BYTE buf[4];
	if (dio->DoRead(startPort, portCount, buf) != Success)
		throw(Exception("A1730_BioDAQ::ReadOut: не могу прочитать плату"));
	DWORD bufOut = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (bufOut);
}

// ---------------------------------------------------------------------------
void A1730_BioDAQ::Write(DWORD _v)
{
	BYTE buf[4];
	buf[0] = _v & 0xFF;
	buf[1] = _v >> 8 & 0xFF;
	buf[2] = _v >> 16 & 0xFF;
	buf[3] = _v >> 24 & 0xFF;
	if (dio->DoWrite(startPort, portCount, buf) != Success)
		throw(Exception("A1730_BioDAQ::Write: не могу записать на плату"));
}
// ---------------------------------------------------------------------------
#endif
