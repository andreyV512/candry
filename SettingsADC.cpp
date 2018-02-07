// ---------------------------------------------------------------------------

#include <vcl.h>
#include "SettingsADC.h"
#include "uFunctions.h"
#pragma hdrstop
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma link "uFRSetADC"
#pragma resource "*.dfm"
TFormSettADC *FormSettADC;

// ---------------------------------------------------------------------------
__fastcall TFormSettADC::TFormSettADC(TComponent* Owner,TIniFile *_ini,
	Result* _result) : TForm(Owner)
{
	ini=_ini;
	result=_result;
	KeyPreview=true;
	// создаем эдиты динамические
	int vmar=50,hmar=55;
	int height=27,width=60;
	int tab=5;
	for(int i=0;i<Globals_max_sensors;i++)
	{
		// инициализируем captions, edits
		arl[i]=new TLabel(this);
		arl[i]->Parent=gbSensors;
		arl[i]->SetBounds(16,vmar+i*(height+10),width,height);
		arl[i]->Visible=false;
		arl[i]->Caption="Датчик "+IntToStr(i+1);

		arA[i]=new TEdit(this);
		arA[i]->Parent=gbSensors;
		arA[i]->SetBounds(hmar+(1)*(width),vmar+i*(height+10),width,
			height);
		arA[i]->Name=L"A"+IntToStr(i);
		arA[i]->TabOrder=(short)(tab+i);
		arA[i]->Visible=false;

		arB[i]=new TEdit(this);
		arB[i]->Parent=gbSensors;
		arB[i]->SetBounds(hmar+(2)*(width)+10,vmar+i*(height+10),
			width,height);
		arB[i]->Name=L"B"+IntToStr(i);
		arB[i]->TabOrder=tab+i+2*Globals_max_sensors;
		arB[i]->Visible=false;

		fadc[i]=new TFRSetADC(tabSheetEKE,i,ini);
		fadc[i]->Parent=tabSheetEKE;
		fadc[i]->Left=0;
		fadc[i]->Top=vmar+i*(height+10)-10;
		fadc[i]->Width=tabSheetEKE->Width;
		fadc[i]->Name=L"fadc"+IntToStr(i);
		fadc[i]->MinE->Left=lMinEnergy->Left;
		fadc[i]->MaxE->Left=lMaxEnergy->Left;
		fadc[i]->EnMax->Left=lEnergyToMax->Left;
		fadc[i]->MaxMin->Left=lSensorsMaxToMin->Left;
		fadc[i]->MaxPeaks->Left=lMaxPeaks->Left;
		fadc[i]->DJProcent->Left=lDJProcent->Left;
		fadc[i]->DJIndex->Left=lDJIndex->Left;
	}

	tbSynchroLevel->SetTick(-100);
	tbSynchroLevel->SetTick(100);
	tbSynchroLevel->Refresh();

}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::bExitClick(TObject *Sender)
{
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormClose(TObject *Sender,TCloseAction &Action)
{
	UnregisterHotKey(Handle, // Handle окна
		0x00F); // наш идентификатор горячего ключа
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::EditSizeBufKeyPress(TObject *Sender,wchar_t &Key)
{
	if(!((Key>='0'&&Key<='9')||Key==VK_BACK))
		Key=0x00; // Не даем нажать ничего кроме цифр и BackSpace
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormKeyPress(TObject *Sender,wchar_t &Key)
{
	if(Key==27)
		Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::ApplicationEvents1Message(tagMSG &Msg,
	bool &Handled)
{
	// Проверяем на нажатие сочетания клавиш Ctrl+Alt+Enter
	if(Msg.message==WM_HOTKEY) // сообщение наше
	{
		if(Msg.wParam==0x00F) // идентификатор наш
		{
			if(cbSensorsNumber->Enabled)
				SetControlsAble(false);
			else
				SetControlsAble(true);
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormActivate(TObject *Sender)
{
	// Резервируем сочетание горячих клавиш Ctrl+Alt+Enter, для скрытия/открытия доп. настроек
	bool RHKret=RegisterHotKey(Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00F, // УСЛОВНЫЙ идентификатор горячего ключа
		MOD_ALT+MOD_CONTROL, // модификаторы
		VK_RETURN); // код клавиши

	for(int i=0;i<Globals_max_sensors;i++)
	{
		arA[i]->Text=ini->ReadFloat("SensorsRudShel",
			"A"+IntToStr(i),0.066);
		arB[i]->Text=ini->ReadFloat("SensorsRudShel","B"+IntToStr(i),
			-0.069);
		fadc[i]->LoadSettings();
	}

	// считываем настройки из ini файла
	cbInaccuracy->ItemIndex=cbInaccuracy->Items->IndexOf
		(ini->ReadInteger("Mathematics","Inaccuracy",1));
	cbPeaksCount->ItemIndex=cbPeaksCount->Items->IndexOf
		(ini->ReadInteger("Mathematics","PeaksCount",0));
	eMaxToMin->Text=ini->ReadFloat("Mathematics","MaxToMin",0.1666667);
	cbSmartAnalysis->Checked=ini->ReadBool("Mathematics","SmartAnalysis",0);
	tbSmartBegin->Position=ini->ReadInteger("Mathematics","SmartBegin",0);
	tbSmartEnd->Position=ini->ReadInteger("Mathematics","SmartEnd",0);
	pcMathLogics->ActivePageIndex=
		ini->ReadInteger("Mathematics","Logic",0); // 0 - GAK, 1 - EKE
	cbACFDivider->ItemIndex=cbACFDivider->Items->IndexOf
		(ini->ReadInteger("Mathematics","ACFDivider",1));

	cbGain->ItemIndex=cbGain->Items->IndexOf(ini->ReadInteger("CardsRudShel",
		"SignalGain",1));
	cbFrequency->ItemIndex=cbFrequency->Items->IndexOf
		(ini->ReadFloat("CardsRudShel","Frequency",25.0));
	cbSensorsNumber->ItemIndex=cbSensorsNumber->Items->IndexOf
		(ini->ReadInteger("CardsRudShel","SensorsNumber",3));
	cbSamplesCount->ItemIndex=cbSamplesCount->Items->IndexOf
		(ini->ReadInteger("CardsRudShel","SamplesCount",484));
	tbSynchroLevel->Position=ini->ReadInteger("CardsRudShel",
		"SynchroLevel",20);
	eStartDelay->Text=ini->ReadInteger("CardsRudShel","StartDelay",20);
	eStrobesPerPacket->Text=ini->ReadInteger("CardsRudShel",
		"StrobesPerPacket",101);
	cbSynchroFront->Checked=ini->ReadBool("CardsRudShel",
		"SynchroFront",true);


	eTestTime->Text=ini->ReadInteger("delays","TestTime",20);

	csAmperage->Value=ini->ReadInteger("Solenoid","amperageChannel",3);
	csVoltage->Value=ini->ReadInteger("Solenoid","voltageChannel",5);
	eResistSolenoid->Text=
		FloatToStr(ini->ReadFloat("Solenoid","ResistSolenoid",5.0));

	SetControlsAble(false);
	cbSensorsNumberChange(Sender);
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::bRecalculateTubeClick(TObject *Sender)
{
	if(result==NULL)
		return;
	if(result->zone.Count()==0)
		return;
	bExit->Enabled=false;
	bRecalculateTube->Enabled=false;

	int time=GetTickCount();
	bool RagACF=ini->ReadBool("Mathematics","RagACF",0);
	result->ReCalc(RagACF,*Globals_mathSettings,ini);
	int endTime=GetTickCount();
	String caption=String().sprintf(L"Время выполнения расчёта - %d мс",
		endTime-time);
	bExit->Enabled=true;
	bRecalculateTube->Enabled=true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::bSaveClick(TObject *Sender)
{
	Globals_adcSettings.measureSize=cbSamplesCount->Text.ToInt();
	Globals_adcSettings.sensorCount=cbSensorsNumber->Text.ToInt();
	Globals_adcSettings.quantFrequency=(float) cbFrequency->Text.ToDouble();
	Globals_adcSettings.syncLevel=tbSynchroLevel->Position;
	Globals_adcSettings.ampGain=cbGain->Text.ToInt();
	Globals_adcSettings.startDelay=eStartDelay->Text.ToInt();
	Globals_adcSettings.syncFront=cbSynchroFront->Checked;
	Globals_adcSettings.strobesPerPacket=eStrobesPerPacket->Text.ToInt();
	Globals_adcSettings.SaveSettings(ini);

	ini->WriteInteger("Solenoid","amperageChannel",csAmperage->Value);
	ini->WriteInteger("Solenoid","voltageChannel",csVoltage->Value);
	ini->WriteFloat("Solenoid","ResistSolenoid",
		(double)StrToFloat(eResistSolenoid->Text));



	ini->WriteInteger("delays","TestTime",StrToInt(eTestTime->Text));

	ini->WriteInteger("Mathematics","Logic",pcMathLogics->ActivePageIndex);
	ini->WriteInteger("Mathematics","Inaccuracy",cbInaccuracy->Text.ToInt());
	ini->WriteInteger("Mathematics","PeaksCount",cbPeaksCount->Text.ToInt());
	ini->WriteFloat("Mathematics","MaxToMin",eMaxToMin->Text.ToDouble());
	ini->WriteBool("Mathematics","SmartAnalysis",cbSmartAnalysis->Checked);
	ini->WriteInteger("Mathematics","SmartBegin",tbSmartBegin->Position);
	ini->WriteInteger("Mathematics","SmartEnd",tbSmartEnd->Position);
	ini->WriteInteger("Mathematics","ACFDivider",cbACFDivider->Text.ToInt());
	// Globals_rtube.SetAllDividers(cbACFDivider->Text.ToInt());

	// MathSettings mathSettings;
	Globals_mathSettings->setPrecision(cbInaccuracy->Text.ToInt());
	Globals_mathSettings->setSensorCount((uint8_t)cbSensorsNumber->Text.ToInt());
	Globals_mathSettings->setMinMaxThicknesses(Globals_typesize.min_detected,
		Globals_typesize.max_detected);
	Globals_mathSettings->setLogics(pcMathLogics->ActivePageIndex);

	for(int i=0;i<this->cbSensorsNumber->Text.ToInt();i++)
	{
		double a=arA[i]->Text.ToDouble(),b=arB[i]->Text.ToDouble();

		calibrationCoefficients cc(a,b);
		Globals_mathSettings->setCalibrationCoefficients((uint8_t)i,cc);

		Globals_mathSettings->setMinEnergy((uint8_t)i,
			(double)StrToFloat(fadc[i]->MinE->Text));
		Globals_mathSettings->setMaxEnergy((uint8_t)i,
			(double)StrToFloat(fadc[i]->MaxE->Text));
		Globals_mathSettings->setMaxToMin((uint8_t)i,
			(double)StrToFloat(fadc[i]->MaxMin->Text));
		Globals_mathSettings->setEnergyToMax((uint8_t)i,
			(double)StrToFloat(fadc[i]->EnMax->Text));
		Globals_mathSettings->setMaxPeaks((uint8_t)i,
			StrToInt(fadc[i]->MaxPeaks->Text));
		Globals_mathSettings->setDJProcent((uint8_t)i,
			StrToInt(fadc[i]->DJProcent->Text));
		Globals_mathSettings->setDJIndex((uint8_t)i,
			(double)StrToFloat(fadc[i]->DJIndex->Text));

		ini->WriteFloat("SensorsRudShel","A"+IntToStr(i),a);
		ini->WriteFloat("SensorsRudShel","B"+IntToStr(i),b);

		fadc[i]->SaveSettings();
	}
	Globals_mathSettings->setMinPeaksCount((uint8_t)cbPeaksCount->Text.ToInt());
	Globals_mathSettings->setSmartMaths(cbSmartAnalysis->Checked);
	Globals_mathSettings->setSmartBegin(tbSmartBegin->Position);
	Globals_mathSettings->setSmartEnd(tbSmartEnd->Position);
	Globals_MaxToMin=eMaxToMin->Text.ToDouble();

}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::cbSensorsNumberChange(TObject *Sender)
{
	int NumSen=this->cbSensorsNumber->Text.ToInt();
	int i;
	for(i=0;i<NumSen;i++)
	{
		arA[i]->Visible=true;
		arB[i]->Visible=true;
		arl[i]->Visible=true;
		fadc[i]->SetVisible(true);
	}
	for(;i<Globals_max_sensors;i++)
	{
		arA[i]->Visible=false;
		arB[i]->Visible=false;
		arl[i]->Visible=false;
		fadc[i]->SetVisible(false);
	}
}

// ---------------------------------------------------------------------------
void TFormSettADC::SetControlsAble(bool Enable)
{
	for(int i=0;i<Globals_max_sensors;i++)
	{
		arA[i]->Enabled=Enable;
		arB[i]->Enabled=Enable;
		fadc[i]->SetEnable(Enable);
	}
	csAmperage->Enabled=Enable;
	csVoltage->Enabled=Enable;
	eResistSolenoid->Enabled=Enable;
	cbSensorsNumber->Enabled=Enable;
	cbInaccuracy->Enabled=Enable;
	cbPeaksCount->Enabled=Enable;
	eMaxToMin->Enabled=Enable;
	cbSmartAnalysis->Enabled=Enable;
	tbSmartBegin->Enabled=Enable;
	tbSmartEnd->Enabled=Enable;
	cbSamplesCount->Enabled=Enable;
	cbGain->Enabled=Enable;
	cbFrequency->Enabled=Enable;
	tbSynchroLevel->Enabled=Enable;
	eStartDelay->Enabled=Enable;
	eStrobesPerPacket->Enabled=Enable;
	cbSynchroFront->Enabled=Enable;
	cbACFDivider->Enabled=Enable;
	eTestTime->Enabled=Enable;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSettADC::tbSynchroLevelChange(TObject *Sender)
{
	lSynchroLevel->Caption=tbSynchroLevel->Position;
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::eResistSolenoidKeyPress(TObject *Sender,
	wchar_t &Key)
{
	if(!((Key>='0'&&Key<='9')||(Key==VK_BACK)||(Key==',')))
		Key=0x00;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSettADC::FormCreate(TObject *Sender)
{
	LoadFormPos(this,ini);
}
// ---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormDestroy(TObject *Sender)
{
	SaveFormPos(this,ini);
}
// ---------------------------------------------------------------------------
