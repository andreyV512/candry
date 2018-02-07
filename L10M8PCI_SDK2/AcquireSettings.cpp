//---------------------------------------------------------------------------
#pragma hdrstop
#include "AcquireSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
AcquireSettings::AcquireSettings(void)
{
	section="CardsRudShel";
	measureSize=474;
	sensorCount=3;
	quantFrequency=(float)(25*1.0e6);
	syncLevel=40;
	ampGain=1;
	strobesPerPacket=100;
	startDelay=0;
	syncFront=true;
}
AcquireSettings::AcquireSettings(TIniFile* _ini)
{
	section="CardsRudShel";
	LoadSettings(_ini);
}
void AcquireSettings::LoadSettings(TIniFile* _ini)
{
	measureSize=_ini->ReadInteger(section,"SamplesCount",484);
	sensorCount=_ini->ReadInteger(section,"SensorsNumber",3);
	quantFrequency=(float)_ini->ReadFloat(section,"Frequency",25.0);
	syncLevel=_ini->ReadInteger(section,"SynchroLevel",20);
	ampGain=_ini->ReadInteger(section,"SignalGain",1);
	strobesPerPacket=_ini->ReadInteger(section,"StrobesPerPacket",102);
	startDelay=_ini->ReadInteger(section,"StartDelay",20);
	syncFront=_ini->ReadBool(section,"SynchroFront",true);
}
void AcquireSettings::SaveSettings(TIniFile* _ini)
{
	_ini->WriteInteger(section,"SamplesCount",measureSize);
	_ini->WriteInteger(section,"SensorsNumber",sensorCount);
	_ini->WriteFloat(section,"Frequency",quantFrequency);
	_ini->WriteInteger(section,"SynchroLevel",syncLevel);
	_ini->WriteInteger(section,"SignalGain",ampGain);
	_ini->WriteInteger(section,"StrobesPerPacket",strobesPerPacket);
	_ini->WriteInteger(section,"StartDelay",startDelay);
	_ini->WriteBool(section,"SynchroFront",syncFront);
}


