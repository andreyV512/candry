// ---------------------------------------------------------------------------
#pragma hdrstop
#include "A1730.h"
#include <Forms.hpp>
#include "Protocol.h"
#include "A1730_DAQNavi.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
A1730* a1730 = NULL;

#ifdef A1730_DAQNaviR
A1730::A1730(int _DevNum, AnsiString _file_name, TIniFile* _ini)
	: A1730_DAQNavi(_DevNum)
#endif
#ifdef A1730_virtualR
A1730::A1730(int _DevNum, AnsiString _file_name, TIniFile* _ini)
	: A1730_virtual(_DevNum)
#endif
{
	startPort = 0;
	portCount = 4;
	if (!_ini->ValueExists("Default", "InversePCHA"))
		_ini->WriteBool("Default", "InversePCHA", false);
	if (!_ini->ValueExists("Default", "InversePCHRUN"))
		_ini->WriteBool("Default", "InversePCHRUN", false);
//---------------------------------------------
	period = 50;
	cs = new TCriticalSection();

	TIniFile *ini = new TIniFile(_file_name);
	TStringList *sections = new TStringList();
	TStringList *keys = new TStringList();
	ini->ReadSections(sections);
	if (sections->Count == 0)
	{
		AnsiString a = "Signals::Signals: файл ";
		a += _file_name;
		a += " либо не существует, либо пуст";
		throw(Exception(a));
	}
	for (int i = 0; i < sections->Count; i++)
	{
		AnsiString Sect = sections->Strings[i];
		bool in = Sect.SubString(8, 3) == "IN_";
		if (Sect.SubString(1, 5) == "ACard")
		{
			ini->ReadSection(Sect, keys);
			for (int j = 0; j < keys->Count; j++)
			{
				AnsiString Key = keys->Strings[j];
				AnsiString MapKey = ini->ReadString(Sect, Key, "");
				M.Add(new CSignal(MapKey, in, keys->Strings[j].ToInt(),
					WriteSignals, cs, Wait));
			}
		}
	}
	delete keys;
	delete sections;
	delete ini;
	iPCHA = Find("ПЧ А");
	iCC = Find("Цепи управления");
	iPCHRUN = Find("ПЧ RUN");
	iSTROBE = Find("Строб Зоны");
	iCYCLE = Find("Цикл");
	iREADY = Find("Готовность");
	iCONTROL = Find("Контроль");

	oPCHPOW = Find("Питание ПЧ");
	oSCANPOW = Find("Питание СУ");
	oSOLPOW = Find("Питание соленоид");
	oWORK = Find("Работа");
	oMEAS = Find("Измерение");
	oSHIFT = Find("Перекладка");
	oSTROBE = Find("Строб");
	oRESULT = Find("Результат");
	alarmCycleOn = false;
	OnFront = NULL;
	IsAlarm = false;
	latchError=false;
}

__fastcall A1730::~A1730(void)
{
	TThread::Terminate();
	TThread::WaitFor();
	delete cs;
}

void _fastcall A1730::Execute()
{
	while (true)
	{
		if (Terminated)
			break;
		ReadSignals();
		Sleep(period);
		if (Terminated)
			break;
	}
}

void A1730::SetPeriod(int _period)
{
	cs->Enter();
	period = _period;
	cs->Leave();
}

CSignal* A1730::Find(AnsiString _name)
{
	for (int i = 0; i < M.Count(); i++)
	{
		if (M[i]->name == _name)
			return (M[i]);
	}
	AnsiString a = "Signals::Find: сигнал ";
	a += _name;
	a += " не найден";
	throw(Exception(a));
}

void A1730::ReadSignals(void)
{
	cs->Enter();
	{
		DWORD buf = Read();
		CSignal* p;
		bool value;
		DWORD tick = GetTickCount();
		for (int i = 0; i < M.Count(); i++)
		{
			p = M[i];
			if (!p->in)
				continue;
			value = (buf & (((DWORD)1) << p->index)) != 0;
			p->value_prev = p->value;
			if (value != p->value)
			{
				p->value = value;
				p->last_changed = tick;
			}
		}
		Alarm();
		SendFront(tick);
		Latch* lp;
		for (int i = 0; i < L.Count(); i++)
		{
			Latch* lp = L[i];
			if (!lp->terminate)
			{
				if (lp->signal->value == lp->value)
				{
					lp->terminate = true;
					lp->reason = "Ok";
					lp->reason.Unique();
				}
			}
			if (lp->terminate)
				lp->signal->event->SetEvent();
		}
		Drop();

	} cs->Leave();
}

void A1730::WriteSignals(void)
{
	cs->Enter();
	{
		DWORD buf = ReadOut();
		CSignal* p;
		for (int i = 0; i < M.Count(); i++)
		{
			p = M[i];
			if (p->in)
				continue;
			if (p->value)
				buf |= ((DWORD)1) << p->index;
			else
				buf &= ~(((DWORD)1) << p->index);
		}
		Write(buf);
	} cs->Leave();
}

AnsiString A1730::Wait(bool _value, CSignal* _signal, DWORD _tm)
{
	if(latchError)
		return("Состояние Аварии");
	Latch* lp;
	cs->Enter();
	{
		lp = L.Add(new Latch(_value, _signal));
	} cs->Leave();
	bool noSignaled = _signal->event->WaitFor(_tm) != wrSignaled;
	AnsiString ret;
	cs->Enter();
	{
		if (noSignaled)
			ret = "Не дождались";
		else
		{
//			Latch* zzz = lp;
			ret = lp->reason;
		}
		_signal->event->ResetEvent();
		if (lp == NULL)
			throw(Exception("A1730::Wait: lp==NULL"));
		if (lp->reason == NULL)
			throw(Exception("A1730::Wait: lp->reason==NULL"));
		if (lp == 0)
			throw(Exception("A1730::Wait: lp==0"));
		if (lp->reason == 0)
			throw(Exception("A1730::Wait: lp->reason==0"));

		L.Remove(lp);
	} cs->Leave();
	return (ret);
}

void A1730::Alarm(void)
{
	if (!IsAlarm)
		return;
	if (iPCHA->WasConst0(false, 50))
	{
		Latchterminate0("Ошибка частотника! Нет сигнала ПЧ А");
		IsAlarm = false;
		return;
	}
	if (iCC->WasConst0(false, 50))
	{
		Latchterminate0("Нет сигнала цепи управления!");
		IsAlarm = false;
		return;
	}
	if (alarmCycleOn)
	{
		if (!iCYCLE->value)
		{
			Latchterminate0("Пропал сигнал Цикл");
			IsAlarm = false;
			return;
		}
	}
	// Цепи управления», «Работа», «ПЧ А», отсутствие сигнала «ПЧ RUN», воздух, ПЧ В
}

void A1730::AlarmCycleOn(bool _v)
{
	cs->Enter();
	{
		alarmCycleOn = _v;
	} cs->Leave();
}

void A1730::SendFront(unsigned int _tick)
{
	if (OnFront == NULL)
		return;
	if (iSTROBE->value == true && iSTROBE->value_prev == false)
		OnFront(_tick);
}

void A1730::SetOnFront(OnFrontDef _OnFront)
{
	cs->Enter();
	{
		OnFront = _OnFront;
	} cs->Leave();
}

void A1730::Latchterminate(AnsiString _msg)
{
	cs->Enter();
	{
		Latchterminate0(_msg);
	} cs->Leave();
}

void A1730::Latchterminate0(AnsiString _reason)
{
	for (int i = 0; i < L.Count(); i++)
	{
		Latch* lp = L[i];
		if (!lp->terminate)
		{
			lp->terminate = true;
			lp->reason = _reason;
			lp->reason.Unique();
		}
	}
}

void A1730::SetAlarm(bool _IsAlarm)
{
	cs->Enter();
	{
		IsAlarm = _IsAlarm;
	} cs->Leave();
}

void A1730::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}
void A1730::Drop(void)
{
	if(oSTROBE->WasConst0(true,50))
		oSTROBE->Set0(false);
	if(oRESULT->WasConst0(true,50))
		oRESULT->Set0(false);
}
void A1730::ClearLatchError(void)
{
	cs->Enter();
	{
		latchError = false;
	} cs->Leave();
}
void A1730::SetLatchError(void)
{
	cs->Enter();
	{
		latchError = true;
	} cs->Leave();
}


