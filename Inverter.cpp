// ------------------------------------------------------------------------------

#pragma hdrstop

#include "Inverter.h"
#include "Global.h"
#include "Protocol.h"

// ------------------------------------------------------------------------------

#pragma package(smart_init)
Inverter *frConverter;
Inverter::Inverter(TIniFile* ini)
{
	AnsiString section="Default";
	if(!ini->ValueExists(section,"InverterAbonent"))
		ini->WriteInteger(section,"InverterAbonent",1);
	if(!ini->ValueExists(section,"InverterPortName"))
		ini->WriteString(section,"InverterPortName","COM10");
	if(!ini->ValueExists(section,"InverterBaudRate"))
		ini->WriteInteger(section,"InverterBaudRate",19200);
	if(!ini->ValueExists(section,"InverterDataBits"))
		ini->WriteInteger(section,"InverterDataBits",7);
	if(!ini->ValueExists(section,"InverterStopBits"))
		ini->WriteInteger(section,"InverterStopBits",0);
	if(!ini->ValueExists(section,"InverterParity"))
		ini->WriteInteger(section,"InverterParity",2);
	if(!ini->ValueExists(section,"InverterReadIntervalTimeout"))
		ini->WriteInteger(section,"InverterReadIntervalTimeout",500);
	if(!ini->ValueExists(section,"InverterReadTotalTimeoutConstant"))
		ini->WriteInteger(section,"InverterReadTotalTimeoutConstant",500);
	if(!ini->ValueExists(section,"InverterReadTotalTimeoutMultiplier"))
		ini->WriteInteger(section,"InverterReadTotalTimeoutMultiplier",0);
	if(!ini->ValueExists(section,"InverterMRS"))
		ini->WriteBool(section,"InverterMRS",true);

	inverter=new CInv();

	inverter->abonent=ini->ReadInteger(section,"InverterAbonent",1);
	inverter->port=ini->ReadString(section,"InverterPortName","COM10");
	inverter->BaudRate=ini->ReadInteger(section,"InverterBaudRate",115200);
	inverter->ByteSize=ini->ReadInteger(section,"InverterDataBits",8);
	inverter->StopBits=ini->ReadInteger(section,"InverterStopBits",0);
	inverter->Parity=ini->ReadInteger(section,"InverterParity",0);

	inverter->ReadIntervalTimeout=ini->ReadInteger(section,"InverterReadIntervalTimeout",500);
	inverter->ReadTotalTimeoutConstant=ini->ReadInteger(section,"InverterReadTotalTimeoutConstant",500);
	inverter->ReadTotalTimeoutMultiplier=ini->ReadInteger(section,"InverterReadTotalTimeoutMultiplier",0);
//	inverter->OnProtocol=OnProtocol;

	mrs=ini->ReadBool(section,"InverterMRS",true);
	testThread=NULL;
	inverter->Open();
	cs=new TCriticalSection();
}
Inverter::~Inverter()
{
	if(testThread!=NULL)
		testThread->Stop();
	delete cs;
	delete inverter;
}
// ------------------------------------------------------------------------------
bool Inverter::setParameterSpeed(int number,int value)
{
	if((number<4)||(number>6))
		return false;
	bool ret;
	cs->Enter();
	{
		ret=inverter->SetFrequency(IntToStr(80+number),value*100);
	}
	cs->Leave();
	return ret;
}
// ------------------------------------------------------------------------------
int Inverter::getParameterSpeed(int number)
{
	int ret;
	cs->Enter();
	{
		ret=(int)inverter->GetFrequency("0"+IntToStr(number))/100;
	}
	cs->Leave();
	return (ret);
}
// ------------------------------------------------------------------------------
bool Inverter::startRotation()
{
//	ResetErrors();
	if(testThread!=NULL)
		return(true);
	bool ret;
	cs->Enter();
	{
		inverter->Reset9966();
		Sleep(1000);
		inverter->SetMode(0);
		inverter->oSTF=true;
		inverter->oRH=true;
//		inverter->oMRS=mrs;
		inverter->oMRS=false;
		ret=inverter->StateWrite();
	}
	cs->Leave();
	if(!ret)
	{
		TPr::pr("ошибка  "+inverter->error_msg);
		return false;
	}
	testThread=new rotationThread(inverter,cs);
	return true;
}
// ------------------------------------------------------------------------------
bool Inverter::stopRotation()
{
	if(testThread==NULL)
		return(true);
	bool ret;
	cs->Enter();
	{
		inverter->oSTF=false;
		inverter->oRH=false;
		ret=inverter->StateWrite();
	}
	cs->Leave();
	testThread->Stop();
	delete testThread;
	testThread=NULL;
	if(!ret)
	{
		TPr::pr("ошибка  "+inverter->error_msg);
		return false;
	}
	return true;
}
// ------------------------------------------------------------------------------
bool Inverter::NETManage()
{
	bool ret;
	cs->Enter();
	{
		ret=inverter->SetMode(2);
	}
	cs->Leave();
	return ret;
}
// ------------------------------------------------------------------------------
bool Inverter::ResetErrors()
{
	bool ret;
	cs->Enter();
	{
		ret=inverter->Reset9696();
	}
	cs->Leave();
	return ret;
}
bool Inverter::stateRead()
{
	bool ret;
	cs->Enter();
	{
		ret=inverter->StateRead();
	}
	cs->Leave();
	return ret;
}
void Inverter::OnProtocol(AnsiString _msg)
{
	TPr::pr(_msg);
}

// ==============================================================================
__fastcall rotationThread::rotationThread(CInv* _inv,TCriticalSection* _cs)
{
	inverter=_inv;
	cs=_cs;
}
// ------------------------------------------------------------------------------
void rotationThread::Stop()
{
	Terminate();
	WaitFor();
}
// ------------------------------------------------------------------------------
void __fastcall rotationThread::Execute()
{
	for(;;)
	{
		if(Terminated)
			break;
		cs->Enter();
		{
			inverter->StateRead();
		}
		cs->Leave();
		if(Terminated)
			break;
		Sleep(500);
		if(Terminated)
			break;
	}
}
// ------------------------------------------------------------------------------

