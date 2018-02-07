// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TestMode.h"
#include "Global.h"
#include "DataAquirer.h"
#include "Protocol.h"
#include "a1730.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

TestMode::TestMode(int _period,RawStrobes* _rawStrobes,ADCBoards* _adcBoards) : TThread(false)
{
	period=_period;
	startTick=GetTickCount();
	rawStrobes=_rawStrobes;
	adcBoards=_adcBoards;
}

void __fastcall TestMode::Execute()
{
	a1730->ClearLatchError();
	pr("Зашли в тест");
	rawStrobes->Clear();
#ifndef NO_A1730
	if(!a1730->iCC->Get())
	{
		Application->MessageBoxW(L"Включите цепи управления!",L"Внимание",
			MB_ICONWARNING);
		rawStrobes->SetTestComplete();
		return;
	}
#endif
	if(!adcBoards->initSettings(&Globals_adcSettings))
		throw(Exception("TestMode::Execute: Не удалось инициализировать платуь плату rudshel"));
	pr("Врубаем соленоиды");
	a1730->oSOLPOW->Set(true);

	DataAcquirer* da0=new DataAcquirer(adcBoards->board0,rawStrobes);
	Sleep(500);
	DataAcquirer* da1=new DataAcquirer(adcBoards->board1,rawStrobes);
	Sleep(1000);

	pr("Перед включением питания СУ");
	a1730->oSCANPOW->Set(true);
	rawStrobes->Clear();
	rawStrobes->StartTick(GetTickCount());
	for(;;)
	{
		if(GetTickCount()>startTick+period)
			break;
		// if (GetTickCount() > startTick + period/2)
		// {
		// int* p=0;
		// *p=5;
		// }
		if(Terminated)
			break;
		Sleep(1000);
	}
	rawStrobes->SetTestComplete();
	delete da0;
	delete da1;
	a1730->oSCANPOW->Set(false);
	a1730->oSOLPOW->Set(false);
	pr("Вышли из теста");
//	ToFile();
}

void TestMode::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}
/*
void TestMode::ToFile(void)
{
	FILE* df=fopen("GBuf.txt","w");
	adcBoards->board0->ToFile(df);
	adcBoards->board1->ToFile(df);
	fclose(df);
}
*/
