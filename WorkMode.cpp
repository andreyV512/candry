// --------------------------------------------	-------------------------------

#pragma hdrstop
#include <IniFiles.hpp>

#include "WorkMode.h"
#include "Main.h"
#include "Protocol.h"
#include "LCARD502.h"
#include "A1730.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------

int centralizerSleep = 500; // время задержки перед срабатыванием схвата
// удалить
int SleepTemp = 750;

// -----------------------------------------------------------------------------
__fastcall WorkThreadClass::WorkThreadClass(RawStrobes* _rawStrobes, TIniFile* _ini, ADCBoards* _adcboards)
{
	rawStrobes = _rawStrobes;
	ThickSolenoid = new Solenoid(_ini);
	ini = _ini;
	adcboards = _adcboards;
	ByTemp = ini->ReadBool("Solenoid", "ByTemp", 0);
}

__fastcall WorkThreadClass::~WorkThreadClass(void)
{
	delete ThickSolenoid;
}

void __fastcall WorkThreadClass::Execute()
{
	a1730->ClearLatchError();
	pr("WorkThreadClass");
	a1730->AlarmCycleOn(false);
	a1730->SetAlarm(false);
	NameThreadForDebugging("WorkThread");
	a1730->oPCHPOW->Set(true);
	adcboards->initSettings(&Globals_adcSettings);
	pr("Инициализировали руднев шиляев");
	AnsiString a;
	a = "0 getchVoltage()=";
	a += ThickSolenoid->getchVoltage();
	pr(a);
	WorkMode();
	a = "1 getchVoltage()=";
	a += ThickSolenoid->getchVoltage();
	pr(a);
	return;
}

void WorkThreadClass::WorkMode(void)
{
	DataAcquirer* da0 = NULL;
	DataAcquirer* da1 = NULL;
	// da0 = new DataAcquirer(adcboards->board0, rawStrobes);
	// da1 = new DataAcquirer(adcboards->board1, rawStrobes);
	// Sleep(2000);
	// return;

	AnsiString a;
	a1730->SetAlarm(true);
	stext2 = "Начинаем работу";
	Synchronize(UpdateMainForm);
	pr("-------------------------------");
	pr("Время: " + TimeToStr(Time()));
	pr("Новая труба, режим Работа");
	int abc = 0;
	result = true;
	// DataAcquirer* da0 = NULL;
	// DataAcquirer* da1 = NULL;
	AnsiString reason;
	while (true)
	{
		// reason = "Тест Авария";
		// break;
		if (!a1730->iCC->Get())
		{
			reason = "Не включены уцепи управления";
			break;
		}
		// ждем ЦИКЛ
		stext2 = "Ждем сигнал Цикл";
		pr(stext2);
		Synchronize(UpdateMainForm);
		reason = a1730->iCYCLE->Wait(true, 600000);
		if (reason != "Ok")
		{
			if (reason == "Не дождались")
				reason = "Не дождались сигнала Цикл!";
			break;
		}
		pr("Начинаем контролировать цикл");
		// Начинаем контролировать цикл
		a1730->AlarmCycleOn(true);
		pr("Выставили перекладку");
		a1730->oSHIFT->Set(true);

		stext2 = "Ожидание трубы на входе в установку (сигнал \"Готовность\")";
		Synchronize(UpdateMainForm);
		reason = a1730->iREADY->Wait(true, INFINITE);
		if (reason != "Ok")
			break;
		pr("Работа: Есть сигнал \"Готовность\" от ТС");
		pr("Сняли перекладку");
		a1730->oSHIFT->Set(false);

		// включаем вращение модуля
		stext2 = "Включаем вращение блока датчиков";
		Synchronize(UpdateMainForm);
		pr(stext2);

		if (!frConverter->setParameterSpeed(Globals_defaultRotParameter,
			ini->ReadInteger("Type_" + Globals_typesize.name, "InSpeed", 20)))
		{
			reason = "Не удалось выставить скорость вращения!";
			break;
		}

		if (!frConverter->startRotation())
		{
			reason = "Не удалось включить вращение";
			break;
		}
		reason = a1730->iPCHRUN->Wait(true, 6000);
		if (reason != "Ok")
		{
			if (reason == "Не дождались")
				reason = "Не смогли раскрутиться!";
			break;
		}
		Sleep(500);
		pr("включим питание соленоидов");
		a1730->oSOLPOW->Set(true);
		Sleep(1000);
		pr(ThickSolenoid->GetUIR());
		if (!ThickSolenoid->OkU())
		{
			reason = "Напряжение соленоидов вне диапозона - АВАРИЯ!!!";
			a1730->oSOLPOW->Set(false);
			break;
		}
		if (!ByTemp)
		{
			if (!ThickSolenoid->OkResist())
			{
				reason = "Сопротивление соленоидов возрасло - ПЕРЕГРЕВ!!!";
				a1730->oSOLPOW->Set(false);
				break;
			}
		}
		else
		{
			if (!ThickSolenoid->OkTemp())
			{
				reason = "Температура соленоидов возросла - ПЕРЕГРЕВ!!!";
				a1730->oSOLPOW->Set(false);
				break;
			}
		}
		pr("выставляем сигнал Работа");
		a1730->oWORK->Set(true);
		pr("Стартуем платы");
		da0 = new DataAcquirer(adcboards->board0, rawStrobes);   //todo убрать потом
		da1 = new DataAcquirer(adcboards->board1, rawStrobes);
		Synchronize(ClearCharts);
		stext2 = "Ждем сигнал КОНТРОЛЬ";
		pr(stext2);
		Synchronize(UpdateMainForm);

		reason = a1730->iCONTROL->Wait(true, INFINITE);
		if (reason != "Ok")
			break;
		pr("Выставили измерение");
		a1730->oMEAS->Set(true);
		stext2 = "Выполняем сбор данных";
		Synchronize(UpdateMainForm);
		pr("включаем питание сканирующего");
		a1730->oSCANPOW->Set(true);
		rawStrobes->StartTick(GetTickCount());
		pr("Начали ловить стробы");
		a1730->SetOnFront(AddTickStrobe);
		pr("Ждем первого строба");
		reason = a1730->iSTROBE->Wait(true, 5000);
		if (reason != "Ok")
		{
			if (reason == "Не дождались")
				reason = "Не дождались первого строба";
			break;
		}
		if (!frConverter->setParameterSpeed(Globals_defaultRotParameter,
			ini->ReadInteger("Type_" + Globals_typesize.name, "WorkSpeed", 40)))
		{
			reason = "Не удалось выставить рабочую скорость вращения!";
			break;
		}
		stext2 = "Выполняем сбор данных (ждем пропадания сигнала \"Контроль\")";

		// пробуем включать поле тут

		Synchronize(UpdateMainForm);

		reason = a1730->iCONTROL->Wait(false, 40000);
		if (reason != "Ok")
		{
			if (reason == "Не дождались")
				reason = "Не дождались снятия сигнала \"Контроль\"!";
			break;
		}
		pr("Перестали ловить стробы");
		a1730->SetOnFront(NULL);
		pr("Перестали контролировать аварии");
		a1730->SetAlarm(false);

		// тут было выключение соленоидов - убрали в конец цикла чтоб не давало
		// помеху на поперечку

		if (!frConverter->stopRotation())
		{
			reason = "Не удалось остановить вращение";
			break;
		}
		reason = "Ok";
		break;
	}
	/*
	 pr("1111111111111111111111111111111");
	 a = "-40 getchVoltage()=";
	 a += ThickSolenoid->getchVoltage();
	 pr(a);
	 rawStrobes->StopTick(GetTickCount());
	 a = "-41 getchVoltage()=";
	 a += ThickSolenoid->getchVoltage();
	 pr(a);
	 pr("1111111111111111111111111111111");
	 a = "-42 getchVoltage()=";
	 a += ThickSolenoid->getchVoltage();
	 pr(a);
	 */
	frConverter->stopRotation();
	rawStrobes->StopTick(GetTickCount());
	for (; ;)
	{
		if (rawStrobes->IsComplete())
			break;
		Sleep(400);
	}
	pr("WorkThreadClass::WorkMode: дождались завершения rawStrobes");
	// сброс сбора плат, если сбор был начат
	if (da0 != NULL)
	{
		delete da0;
		da0 = NULL;
	}
	if (da1 != NULL)
	{
		delete da1;
		da1 = NULL;
	}
	pr("Перестали ловить стробы в конце");
	a1730->SetOnFront(NULL);
	pr("Перестали контролировать аварии");
	a1730->SetAlarm(false);
	a1730->AlarmCycleOn(false);
	// сбросим управляющие сигналы
	pr("Сняли все сигналы");
	a1730->oSOLPOW->Set(false);
	a1730->oSHIFT->Set(false);
	a1730->oRESULT->Set(false);
	// a1730->oWORK->Set(false);
	a1730->oSCANPOW->Set(false);
	a1730->oMEAS->Set(false);
	if (reason == "Ok")
	{
		Sleep(1000);
		a1730->oWORK->Set(false);
		stext1 = "Режим \"Работа\" завершен успешно";
		stext2 = "";
		result = true;
	}
	else if (reason == "Прервано пользователем")
	{
		a1730->oWORK->Set(false);
		stext1 = "Режим \"Работа\" прерван пользователем";
		frConverter->stopRotation();
		stext2 = reason;
		result = false;
	}
	else
	{
		a1730->oWORK->Set(false);
		stext1 = "Режим \"Работа\" завершен с аварией!";
		frConverter->stopRotation();
		a1730->oPCHPOW->Set(false);
		stext2 = reason;
		result = false;
	}
	pr(stext1);
	pr(stext2);
	Synchronize(UpdateMainForm);
}

// -----------------------------------------------------------------------------
void __fastcall WorkThreadClass::ClearCharts()
{
	MainForm->ClearCharts();
}

void __fastcall WorkThreadClass::UpdateMainForm()
{
	MainForm->StatusBarTop->Panels->Items[1]->Text = stext1;
	MainForm->StatusBarTop->Panels->Items[2]->Text = stext2;
	MainForm->StatusBarTop->Refresh();
	MainForm->work_result = result;
}

void WorkThreadClass::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}

void __fastcall WorkThreadClass::AddTickStrobe(unsigned int _tick)
{
	rawStrobes->AddTickStrobe(_tick);
}
