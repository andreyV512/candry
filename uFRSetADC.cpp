// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFRSetADC.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRSetADC *FRSetADC;
// ---------------------------------------------------------------------------
__fastcall TFRSetADC::TFRSetADC(TComponent* Owner,int _pos,TIniFile* _ini)
	: TFrame(Owner)
{
	pos=_pos;
	ini=_ini;
	section="SensorsRudShel";
}
// ---------------------------------------------------------------------------
void TFRSetADC::SetEnable(bool _v)
{
	MinE->Enabled=_v;
	MaxE->Enabled=_v;
	MaxMin->Enabled=_v;
	EnMax->Enabled=_v;
	MaxPeaks->Enabled=_v;
	DJProcent->Enabled=_v;
	DJIndex->Enabled=_v;
}
// ---------------------------------------------------------------------------
void TFRSetADC::SetVisible(bool _v)
{
	MinE->Visible=_v;
	MaxE->Visible=_v;
	MaxMin->Visible=_v;
	EnMax->Visible=_v;
	MaxPeaks->Visible=_v;
	DJProcent->Visible=_v;
	DJIndex->Visible=_v;
}
// ---------------------------------------------------------------------------
void TFRSetADC::LoadSettings(void)
{
	MinE->Text=ini->ReadFloat(section,"MinE"+IntToStr(pos),5);
	MaxE->Text=ini->ReadFloat(section,"MaxE"+IntToStr(pos),100);
	MaxMin->Text=ini->ReadFloat(section,"MaxMin"+IntToStr(pos),2);
	EnMax->Text=ini->ReadFloat(section,"EnergyToMax"+IntToStr(pos),2.5);
	MaxPeaks->Text=ini->ReadInteger(section,"MaxPeaks"+IntToStr(pos),10);
	DJProcent->Text=ini->ReadInteger(section,"DJProcent"+IntToStr(pos),30);
	DJIndex->Text=ini->ReadFloat(section,"DJIndex"+IntToStr(pos),1.8);
}
// ---------------------------------------------------------------------------
void TFRSetADC::SaveSettings(void)
{
	ini->WriteFloat(section,"MinE"+IntToStr(pos),(double)StrToFloat(MinE->Text));
	ini->WriteFloat(section,"MaxE"+IntToStr(pos),(double)StrToFloat(MaxE->Text));
	ini->WriteFloat(section,"MaxMin"+IntToStr(pos),(double)StrToFloat(MaxMin->Text));
	ini->WriteFloat(section,"EnergyToMax"+IntToStr(pos),(double)StrToFloat(EnMax->Text));
	ini->WriteInteger(section,"MaxPeaks"+IntToStr(pos),StrToInt(MaxPeaks->Text));
	ini->WriteInteger(section,"DJProcent"+IntToStr(pos),StrToInt(DJProcent->Text));
	ini->WriteFloat(section,"DJIndex"+IntToStr(pos),(double)StrToFloat(DJIndex->Text));
}
