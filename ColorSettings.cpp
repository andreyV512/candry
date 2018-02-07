// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ColorSettings.h"
#include "Global.h"
#include "uFunctions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormColor *FormColor;

// ---------------------------------------------------------------------------
__fastcall TFormColor::TFormColor(TComponent* Owner,TIniFile *_ini) : TForm(Owner)
{
	ini = _ini;
}

// ---------------------------------------------------------------------------
void __fastcall TFormColor::FormCreate(TObject *Sender)
{
	LoadFormPos(this,ini);
	PanelBrack->Color = (TColor)ini->ReadInteger(L"Color", L"Brack", 255);
	PanelSecondClass->Color = (TColor)ini->ReadInteger(L"Color", L"SecondClass",16711680);
	PanelValid->Color = (TColor)ini->ReadInteger(L"Color", L"Valid", 41984);
	PanelNotMeasured->Color = (TColor)ini->ReadInteger(L"Color", L"NotMeasured", 0x555555);

	ActiveIn->Color = (TColor)ini->ReadInteger(L"Color", L"ActiveIn", 1503543);
	ActiveOut->Color = (TColor)ini->ReadInteger(L"Color", L"ActiveOut", 465649);

	NotActiveOut->Color = (TColor)ini->ReadInteger(L"Color", L"NotActiveOut",	8421504);
	NotInActive->Color = (TColor)ini->ReadInteger(L"Color", L"NotActiveIn", 12425481);
}

// ---------------------------------------------------------------------------
void __fastcall TFormColor::PanelValidClick(TObject *Sender)
{
	TPanel* Pan = dynamic_cast<TPanel*>(Sender);
	if (ColorDialog->Execute())
		Pan->Color = ColorDialog->Color;
}

// ---------------------------------------------------------------------------
void __fastcall TFormColor::BDefaultClick(TObject *Sender)
{
	this->PanelBrack->Color = (TColor)255;
	this->PanelSecondClass->Color = (TColor)65535;
	this->PanelValid->Color = (TColor)41984;
	this->PanelNotMeasured->Color = (TColor)0x555555;

	this->ActiveIn->Color = (TColor)1503543;
	this->ActiveOut->Color = (TColor)465649;
	this->NotActiveOut->Color = (TColor)8421504;
	this->NotInActive->Color = (TColor)12425481;
}

// ---------------------------------------------------------------------------
void __fastcall TFormColor::BCloseClick(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormColor::BSaveClick(TObject *Sender)
{
	ini->WriteInteger(L"Color", L"Brack", PanelBrack->Color);
	ini->WriteInteger(L"Color", L"Valid", PanelValid->Color);
	ini->WriteInteger(L"Color", L"SecondClass", PanelSecondClass->Color);
	ini->WriteInteger(L"Color", L"NotMeasured", PanelNotMeasured->Color);

	ini->WriteInteger(L"Color", L"ActiveIn", this->ActiveIn->Color);
	ini->WriteInteger(L"Color", L"NotActiveIn", this->NotInActive->Color);
	ini->WriteInteger(L"Color", L"ActiveOut", this->ActiveOut->Color);
	ini->WriteInteger(L"Color", L"NotActiveOut", this->NotActiveOut->Color);
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TFormColor::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TFormColor::FormDestroy(TObject *Sender)
{
	SaveFormPos(this,ini);
}
//---------------------------------------------------------------------------

