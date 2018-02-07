// ---------------------------------------------------------------------------
#include <vcl.h>
#include "Global.h"
#pragma hdrstop
#include "Main.h"
#include "ColorSettings.h"
#include "thickmath.h"
#include <vector.h>
#include "SignalsState.h"
#include "DataAquirer.h"
#include "uFunctions.h"
#include "LCARD502.h"
#include "Protocol.h"
#include "A1730.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using System::TThreadFunc;
#define		DEFAULT_PRECISION_VALUE		(2)

TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	TThread::CurrentThread->NameThreadForDebugging("Main ");

	ini = new TIniFile("..\\..\\Settings\\SettingsThickness.ini");
	Globals_MaxToMin = 0.1666667;
	Tag = 0; // запуск произведен 1й раз очистка буфера не требуется

	LoadFormPos(this, ini);
	if (ini->ReadBool("Default", "ProtocolVisible", false))
		TPr::Show();
	Globals_mathSettings = new MathSettings();
	rawStrobes = NULL;
	calcers = NULL;
	LoadSettings();
	UpdateComboBox();
	int dev_num = ini->ReadInteger("CardNames", "ACard0", -1);
	a1730 = new A1730(dev_num, "..\\..\\Settings\\IO_Management.ini", ini);
	SignalsStateForm = new TSignalsStateForm(this, ini);
	if (ini->ReadBool("Default", "SignalsVisible", false))
		SignalsStateForm->Show();
	// загружаем платы РудШел
	adcboards = ADCBoards::Create(1, 2, pr);
#ifndef NO_ADCBOARDS
	if (adcboards == NULL)
		throw(Exception("TMainForm::FormCreate: Не могу открыть платы L10M8PCI"));
#endif
	cbInterruptView->Checked = ini->ReadBool("Default", "IsInterruptView", false);
	MainFormInit(this);
	KeyPreview = true;
	sms = new SMS(this);
	ClearCharts();
	TimerZone->Interval = ini->ReadInteger("Default", "TimerZonePeriod_ms", 500);
	TestZoneSize = ini->ReadInteger("Default", "TestZoneSize", 100);
	testMode = NULL;
	workthread = NULL;
#ifndef NODEVICES
	a1730->oPCHPOW->Set(true);
	a1730->oSOLPOW->Set(false);
	a1730->oSHIFT->Set(false);
	a1730->oRESULT->Set(false);
	a1730->oWORK->Set(false);
	a1730->oSCANPOW->Set(false);
	a1730->oMEAS->Set(false);
#endif
	SetControlsAble(true);
	lcard = new LCard502();
	SetControlsAble(true);
	frConverter = new Inverter(ini);
	Sleep(1000);
	if (!frConverter->stateRead())
	{
		StatusBarBottom->Panels->Items[0]->Text = "Не удалось прочитать состояние ПЧ";
		StatusBarBottom->Refresh();
		return;
	}
	StatusBarBottom->Panels->Items[0]->Text = "ПЧ подключен";
	StatusBarBottom->Refresh();
}

// ---------------------------------------------------------------------------
void TMainForm::LoadSettings(void)
{
	Globals_typesize.setTypesize(ini->ReadString("Default", "TypeSize", "1"), ini);
	Globals_adcSettings.LoadSettings(ini);
	double t1;
	DrawResults* g = new DrawResults(ini);
	delete g;

	DrawResults::borders[0] = Globals_typesize.borders[0];

	DrawResults::min_good_length = Globals_typesize.min_good_length;

	if (Globals_typesize.borders.Length < 2)
		t1 = 0;
	else
		t1 = Globals_typesize.borders[1];

	eThicknessBorder1->Text = DrawResults::borders[0];
	eThicknessBorder2->Text = t1;
	eMin_Good_Length->Text = DrawResults::min_good_length;

	if (t1 == 0)
		DrawResults::borders.set_length(1);
	else
	{
		DrawResults::borders.set_length(2);
		DrawResults::borders[1] = t1;
	}

	// Считывание настроек, имеющих отношение к вычислению толщины (математике)

	uint8_t sensorCount = (uint8_t)ini->ReadInteger("CardsRudShel", "SensorsNumber", 3);
	Globals_mathSettings->setSensorCount(sensorCount);
	uint8_t precision = (uint8_t)ini->ReadInteger("Mathematics", "Inaccuracy", DEFAULT_PRECISION_VALUE);
	Globals_mathSettings->setPrecision(precision);
	Globals_mathSettings->setMinMaxThicknesses(Globals_typesize.min_detected, Globals_typesize.max_detected);
	Globals_mathSettings->setMinPeaksCount((uint8_t) ini->ReadInteger("Mathematics", "PeaksCount", 0));
	Globals_mathSettings->setSmartMaths(ini->ReadBool("Mathematics", "SmartAnalysis", 0));
	Globals_mathSettings->setSmartBegin((int8_t)ini->ReadInteger("Mathematics", "SmartBegin", 0));
	Globals_mathSettings->setSmartEnd((int8_t)ini->ReadInteger("Mathematics", "SmartEnd", 0));
	Globals_mathSettings->setLogics(ini->ReadInteger("Mathematics", "Logic", 0));

	for (int i = 0; i < sensorCount; i++)
	{
		double a, b;
		b = ini->ReadFloat("SensorsRudShel", "B" + IntToStr(i), -0.069);
		a = ini->ReadFloat("SensorsRudShel", "A" + IntToStr(i), 0.066);

		calibrationCoefficients cc(a, b);
		Globals_mathSettings->setCalibrationCoefficients((uint8_t)i, cc);
		Globals_mathSettings->setMinEnergy((uint8_t)i, ini->ReadFloat("SensorsRudShel", "MinE" + IntToStr(i), 30));
		Globals_mathSettings->setMaxEnergy((uint8_t)i, ini->ReadFloat("SensorsRudShel", "MaxE" + IntToStr(i), 800));
		Globals_mathSettings->setMaxToMin((uint8_t)i, ini->ReadFloat("SensorsRudShel", "MaxMin" + IntToStr(i), 3));
		Globals_mathSettings->setEnergyToMax(i, ini->ReadFloat("SensorsRudShel", "EnergyToMax" + IntToStr(i), 3));

		Globals_mathSettings->setMaxPeaks(i, ini->ReadInteger("SensorsRudShel", "MaxPeaks" + IntToStr(i), 10));
		Globals_mathSettings->setDJProcent((uint8_t)i, ini->ReadInteger("SensorsRudShel",
			"DJProcent" + IntToStr(i), 30));
		Globals_mathSettings->setDJIndex((uint8_t)i, ini->ReadFloat("SensorsRudShel", "DJIndex" + IntToStr(i), 1.8));

	}
	if (calcers != NULL)
		delete calcers;
	if (rawStrobes != NULL)
		delete rawStrobes;
	int max_packets = ini->ReadInteger("CardsRudShel", "MaxPackets", 600);
	int MeasureFrequency_hz = ini->ReadInteger("CardsRudShel", "MeasureFrequency_hz", 100);
	int CalcingWaitPeriod = ini->ReadInteger("CardsRudShel", "CalcingWaitPeriod", 1000);
	int zone_size = ini->ReadInteger("Length", "Zone", 200);
	double thickness_noms = ini->ReadFloat("Type_" + Globals_typesize.name, "thickness_noms", 0);
	rawStrobes = new RawStrobes(max_packets, Globals_adcSettings.strobesPerPacket, Globals_adcSettings.sensorCount,
		Globals_adcSettings.measureSize, MeasureFrequency_hz, CalcingWaitPeriod,
		Globals_typesize.ThicknessDeadZoneStart, Globals_typesize.ThicknessDeadZoneFinish, zone_size,
		Globals_typesize.max_detected, thickness_noms);
	rawStrobes->single_sensor=ini->ReadInteger("CardsRudShel", "SingleSensor", -1);
	int calcers_count = ini->ReadInteger("CardsRudShel", "CalcersCount", 3);
	int calcer_period = ini->ReadInteger("CardsRudShel", "CalcerPeriod", 1000);
	bool RagACF = ini->ReadBool("Mathematics", "RagACF", 0);
	calcers = new Calcers(calcers_count, calcer_period, rawStrobes, RagACF, *Globals_mathSettings, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	Sleep(1000);
#ifndef NODEVICES
	a1730->oPCHPOW->Set(false);
	a1730->oSOLPOW->Set(false);
	a1730->oSHIFT->Set(false);
	a1730->oRESULT->Set(false);
	a1730->oWORK->Set(false);
	a1730->oSCANPOW->Set(false);
	a1730->oMEAS->Set(false);
#endif

	// запись дефолтного всего
	ini->WriteString("Default", "TypeSize", cbTypeSize->Text);
	ini->WriteBool("Default", "IsInterruptView", cbInterruptView->Checked);

	delete frConverter;
	delete sms;
	delete adcboards;
	delete Globals_mathSettings;

	delete a1730;
	delete calcers;
	delete rawStrobes;

#ifndef NODEVICES
	ini->WriteBool("Default", "SignalsVisible", SignalsStateForm->Visible);
#endif
	delete SignalsStateForm;
	ini->WriteBool("Default", "ProtocolVisible", TPr::Visible());
	TPr::Dispose();
	SaveFormPos(this, ini);
	delete ini;
	delete lcard;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MainFormInit(TObject *Sender)
{
	if (ClientWidth < bManage->Left + bManage->Width + 5)
		ClientWidth = bManage->Left + bManage->Width + 5;
	int w = Width;
	int ht = (StatusBarTop->Height) + (FlowPanel->Height) + 80;
	int h = (Height) - (StatusBarBottom->Height * 2) - (FlowPanel->Height) - ht;
	int wm = 20, hm = 75;

	gbThickness->Width = w - wm * 2;
	gbThickness->Left = wm / 2;
	gbThickness->Height = h;
	gbThickness->Top = ht;
	ThicknessChart->Height = h - hm;

	// ThicknessChart->Title->Visible = Globals_isView;
	ThicknessChart->Legend->Visible = false;
	ThicknessChart->LeftAxis->Automatic = false;
	ThicknessChart->LeftAxis->Minimum = 0;
	ThicknessChart->LeftAxis->Maximum = Globals_typesize.max_detected;
	ThicknessChart->BottomAxis->Automatic = false;
	ThicknessChart->BottomAxis->Maximum = Globals_max_zones;
	ThicknessChart->BottomAxis->Minimum = 0;
	ThicknessChart->Zoom->Allow = false;
	ThicknessChart->AllowPanning = TPanningMode::pmNone;

	StatusBarBottom->Panels->Items[0]->Width = w / 4;
	StatusBarBottom->Panels->Items[1]->Width = w / 4;
	StatusBarBottom->Panels->Items[2]->Width = w / 4;
	StatusBarBottom->Panels->Items[3]->Width = w / 4;

	StatusBarTop->Panels->Items[0]->Width = w / 6;
	StatusBarTop->Panels->Items[1]->Width = w / 3;
	StatusBarTop->Panels->Items[2]->Width = w / 2;
	pResult->Left = ThicknessChart->Left;
	pResult->Width = ThicknessChart->Width;
}

// ---------------------------------------------------------------------------

void TMainForm::UpdateComboBox()
{
	cbTypeSize->Items->Clear();
	TStringList * sections = new TStringList();
	ini->ReadSections(sections);
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);

	cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(Globals_typesize.name);
	sections->~TStringList();
	if (cbTypeSize->Items->Count == 0)
		Application->MessageBoxW(L"Ни одного типоразмера не найдено! Создайте их, нажав клавишу F4", L"Предупреждение!",
		MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::cbTypeSizeSelect(TObject *Sender)
{
	ini->WriteString("Default", "TypeSize", cbTypeSize->Text);
	LoadSettings();
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::bTestClick(TObject *Sender)
{
	int TestTime = ini->ReadInteger("Default", "TestTime_s", 10);
	// int TestZoneSize = ini->ReadInteger("Default", "TestZoneSize", 100);

	thickness.clear();
	pr("Блокируем форму");
	SetControlsAble(false, NULL, NULL);
	ClearCharts();
	TPr::Clear();

	StatusBarTop->Panels->Items[1]->Text = L"Идет сбор данных с АЦП";
	StatusBarTop->Panels->Items[2]->Text = L" ";
	StatusBarTop->Refresh();
	bCancel = false;
	rawStrobes->Clear();
	testMode = new TestMode(TestTime * 1000, rawStrobes, adcboards);
	test_timer = true;
	TimerZone->Enabled = true;
}

// ------------------------------------------------------------------------------
void __fastcall TMainForm::bWorkClick(TObject *Sender)
{
	thickness.clear();
	pr("Блокируем форму");
	SetControlsAble(false);
	ClearCharts();
	TPr::Clear();

	StatusBarTop->Panels->Items[1]->Text = "Режим \"Работа\"";
	StatusBarTop->Panels->Items[2]->Text = "Готовим к исходному положению";
	StatusBarTop->Refresh();
	bCancel = false;
	rawStrobes->Clear();
	if (workthread != NULL)
		throw;
	workthread = new WorkThreadClass(rawStrobes, ini, adcboards);
	frConverter->ResetErrors();
	test_timer = false;
	TimerZone->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bViewClick(TObject *Sender)
{
	Result* p = rawStrobes->GetResult();
	if (p == NULL)
		return;
	if (p->zone.Count() == 0)
		return;
	ViewForm = new TViewForm(this, p, ini);
	ViewForm->ShowModal();
	delete ViewForm;
}

// ------------------------------------------------------------------------------
void __fastcall TMainForm::bCancelWorkClick(TObject *Sender)
{
	SetControlsAble(false);
	if (testMode != NULL)
		testMode->Terminate();
	if (workthread != NULL)
		workthread->Terminate();
	a1730->Latchterminate("Прервано пользователем");
	bCancel = true;
	// SetControlsAble(true);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bManageClick(TObject *Sender)
{
	TManageForm* ManageForm = new TManageForm(this, ini);
	ManageForm->ShowModal();
	delete ManageForm;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bGoodTubeClick(TObject *Sender)
{
	TPr::pr("Дефектоскопист сменил результат на \"Годно\"");
	bWorkClick(Sender);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bClass2TubeClick(TObject *Sender)
{
	TPr::pr("Дефектоскопист сменил результат на \"Класс 2\"");
	bWorkClick(Sender);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 112)
		WinExec("hh Help.chm", SW_RESTORE);
	if (Key == '`' || Key == L'ё')
	{
		cbTypeSize->ItemIndex = (cbTypeSize->ItemIndex < cbTypeSize->Items->Count - 1) ?
			(cbTypeSize->ItemIndex + 1) : (0);
		cbTypeSizeSelect(Sender);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuTypeSizeClick(TObject *Sender)
{
	// показать окно с настройками типоразмера
	TSettingsForm* SettingsForm = new TSettingsForm(this, ini);
	SettingsForm->ShowModal();
	delete SettingsForm;
	LoadSettings();
	UpdateComboBox();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuMeasuresClick(TObject *Sender)
{
	TFormSettADC* FormSettADC = new TFormSettADC(this, ini, rawStrobes->GetResult());
	FormSettADC->ShowModal();
	delete FormSettADC;
	thickness.clear();
	Result* result = rawStrobes->GetResult();
	if (result == NULL)
		return;
	for (int i = 0; i < result->zone.Count(); i++)
		thickness.push_back(result->zone[i]->thickness_median);
	PutSummaryResultOnChart(thickness);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuQuitClick(TObject *Sender)
{
	// выход из программы
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuSignalsStateClick(TObject *Sender)
{
	SignalsStateForm->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuSaveTubeClick(TObject *Sender)
{
	if (SaveToFileDialog->Execute())
	{
		if (rawStrobes->SaveResult(SaveToFileDialog->FileName))
			StatusBarTop->Panels->Items[1]->Text = "Файл сохранен";
		else
			StatusBarTop->Panels->Items[1]->Text = "Файл НЕ сохранен!";
	}
}
// ------------------------------------------------------------------------------------

void __fastcall TMainForm::menuLoadTubeClick(TObject *Sender)
{
	if (OpenDialogFromFile->Execute())
	{
		thickness.clear();
		if (rawStrobes->LoadResult(OpenDialogFromFile->FileName))
		{
			StatusBarTop->Panels->Items[1]->Text = "Файл заружен";
			Result* result = rawStrobes->GetResult();
			for (int i = 0; i < result->zone.Count(); i++)
				thickness.push_back(result->zone[i]->thickness_median);
		}
		else
			StatusBarTop->Panels->Items[1]->Text = "Файл НЕ загружен!";
		PutSummaryResultOnChart(thickness);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuTestAdvantechClick(TObject *Sender)
{
	ShellExecute(0, L"open", L"..\\..\\Settings\\IO_Management.exe", 0, 0, SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuF1Click(TObject *Sender)
{
	WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuColorsClick(TObject *Sender)
{
	TFormColor* FormColor = new TFormColor(this, ini);
	FormColor->ShowModal();
	delete FormColor;
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuProtocolClick(TObject *Sender)
{
	TPr::Show();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuTestSMSConnectionClick(TObject *Sender)
{
	if (sms->TestConnection())
		StatusBarTop->Panels->Items[2]->Text = "Связь с дефектоскопом работает";
	else
		StatusBarTop->Panels->Items[2]->Text = "Связи с программой дефектоскопии НЕТ!";
}
// ---------------------------------------------------------------------------

void TMainForm::SetControlsAble(bool state, TButton *exc1, TMenuItem *exc2, TButton *exc3, TButton *exc4)
{
	cbTypeSize->Enabled = state;

	bTest->Enabled = state;
	bView->Enabled = state;
	bWork->Enabled = state;
	bManage->Enabled = state;
	bCancelWork->Enabled = !state;

	eMin_Good_Length->Enabled = state;
	eThicknessBorder1->Enabled = state;
	eThicknessBorder2->Enabled = state;

	menuTube->Enabled = state;
	menuSettings->Enabled = state;
	menuTest->Enabled = state;
	menuTestAdvantech->Enabled = state;
	menuWork->Enabled = state;
	menuManagement->Enabled = state;
	menuQuit->Enabled = state;

	if (exc1 != NULL)
		exc1->Enabled = !state;
	if (exc2 != NULL)
		exc2->Enabled = !state;
	if (exc3 != NULL)
		exc3->Enabled = !state;
	if (exc4 != NULL)
		exc4->Enabled = !state;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::gbThicknessClick(TObject *Sender)
{
	bView->Click();
}

// ---------------------------------------------------------------------------
void TMainForm::PutSummaryResultOnChart(std::vector<double>thickness)
{
	ThicknessChart->Axes->Left->SetMinMax(0.0, Globals_mathSettings->MaxThickness());
	ThicknessChart->Series[0]->ColorEachPoint = true;
	ThicknessChart->Series[0]->Marks->Visible = false;
	((TBarSeries *) ThicknessChart->Series[0])->BarWidthPercent = 100;
	((TBarSeries *) ThicknessChart->Series[0])->OffsetPercent = 50;
	((TBarSeries *) ThicknessChart->Series[0])->SideMargins = true;
	ThicknessChart->Series[0]->Clear();
	// ProtocolForm->SendToProtocol("Перед отрисовкой по зонам. ");
	for (int i = 0; i < Globals_max_zones; i++)
	{
		if (i < (int)thickness.size())
		{
			ThicknessChart->Series[0]->AddXY((double)i, thickness[i], "", DrawResults::GetColor(thickness[i]));
			// pr(AnsiString("PSR=")+thickness[i]);
		}
		else
			ThicknessChart->Series[0]->AddXY(i, 0, "", clWhite);
	}
	// ProtocolForm->SendToProtocol("После отрисовки по зонам. ");

	ThicknessChart->Series[0]->RefreshSeries();
	ThicknessChart->Series[0]->Repaint();
	ThicknessChart->Refresh();
}
// ------------------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// a1730->oPCHPOW->Set(false);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ClearCharts(void)
{
	// ThicknessChart->RemoveAllSeries();
	// ThicknessChart->AddSeries(new TBarSeries(ThicknessChart));
	ThicknessChart->Series[0]->Clear();
	pResult->Caption = "Результат";
	pResult->Color = clBtnFace;
	pLength->Visible = false;
	pVelocity->Visible = false;
	// Globals_isView = false;
	Caption = "БУРАН-5000. Ультразвуковой контроль толщины стенки НКТ.";
	Refresh();
	MainFormInit(this);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::TimerZoneTimer(TObject *Sender)
{
	// pr("TimerZoneTimer");
	TTimer* t = (TTimer*)Sender;
	t->Enabled = false;
	Zone* zone;
	if (test_timer)
		zone = rawStrobes->GetNewZoneTest(TestZoneSize);
	else
		zone = rawStrobes->GetNewZone();
	if (zone != NULL)
	{
		pr("Получили новую зону");
		thickness.push_back(zone->thickness_median);
		PutSummaryResultOnChart(thickness);
		double sms_thickness_median = zone->thickness_median;
		a1730->oSTROBE->Set(true);
		if (DrawResults::IsBrak(zone->thickness_median))
		{
			a1730->oRESULT->Set(true);
			AnsiString a = "oRESULT[";
			a += zone->nn;
			a += "]=";
			a += zone->thickness_median;
			pr(a);
		}
		if (sms_thickness_median >= Globals_typesize.max_detected)
			sms_thickness_median = 10;
		if (!test_timer)
		{
			AnsiString a = "sms_thickness_median=";
			a += sms_thickness_median;
			pr(a);
			sms->SendZoneThickness(zone->nn, sms_thickness_median);
		}
	}
	if (rawStrobes->IsComplete())
	{
		pr("Банк закончил работу");
		a1730->Latchterminate("Банк закончил работу");
		if (testMode != NULL)
		{
			delete testMode;
			testMode = NULL;
		}
		if (workthread != NULL)
		{
			delete workthread;
			workthread = NULL;
		}
		StatusBarTop->Panels->Items[1]->Text = "РАБОТА завершена";
		if (test_timer)
			SetControlsAble(true);
		else
		{
			if (bCancel)
				SetControlsAble(true);
			else
			{
				if (cbInterruptView->Checked)
					SetControlsAble(true);
				else
				{
					if (work_result)
						bWork->Click();
					else
						SetControlsAble(true);
				}
			}
		}
	}
	else
		t->Enabled = true;
}

// ---------------------------------------------------------------------------
void TMainForm::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}
