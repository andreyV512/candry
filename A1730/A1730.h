// ---------------------------------------------------------------------------
#ifndef A1730H
#define A1730H
#include<Classes.hpp>
#include "Signal.h"
#include "uCRListT.h"
#include "Latch.h"
#include<IniFiles.hpp>
#ifdef A1730_DAQNaviR
#include "A1730_DAQNavi.h"
#endif
#ifdef A1730_BioDAQR
#include "A1730_BioDAQ.h"
#endif
#ifdef A1730_virtualR
#include "A1730_virtual.h"
#endif

// ---------------------------------------------------------------------------
#ifdef A1730_DAQNaviR
class A1730 : private A1730_DAQNavi
#endif
#ifdef A1730_BioDAQR
class A1730 : private A1730_BioDAQ
#endif
#ifdef A1730_virtualR
class A1730 : private A1730_virtual
#endif
{
public:
	typedef void __fastcall(__closure*OnEventDef)(void);
	typedef void __fastcall(__closure*OnFrontDef)(unsigned int _tick);
private:

	int period;
	TCriticalSection* cs;
	long startPort;
	long portCount;
	CRListT <CSignal> M;
	CRListT <Latch> L;

	void __fastcall Execute();
	bool GetIn(int _index);
	bool GetOut(int _index);
	void Set(int _index,bool _value);
	void WriteSignals(void);
	void ReadSignals(void);
	CSignal* Find(AnsiString _name);
	AnsiString Wait(bool _value,CSignal* _signal,DWORD _tm);
	void Alarm(void);
	bool alarmCycleOn;
	OnFrontDef OnFront;

	void SendFront(unsigned int _tick);
	void Latchterminate0(AnsiString _reason);
	bool IsAlarm;
	void Drop(void);
	void pr(AnsiString _msg);
	bool latchError;
public:
	A1730(int _DevNum,AnsiString _file_name,TIniFile* _ini);
	__fastcall ~A1730(void);
	void SetPeriod(int _period);
	void ClearLatchError(void);
	void SetLatchError(void);

	CSignal* iPCHA; // ПЧ А
	CSignal* iCC; // Цепи управления
	CSignal* iPCHRUN; // ПЧ RUN
	CSignal* iSTROBE; // Строб Зоны
	CSignal* iCYCLE; // Цикл
	CSignal* iREADY; // Готовность
	CSignal* iCONTROL; // Контроль

	CSignal* oPCHPOW; // Питание ПЧ
	CSignal* oSCANPOW; // Питание СУ
	CSignal* oSOLPOW; // Питание соленоид
	CSignal* oWORK; // Работа
	CSignal* oMEAS; // Измерение
	CSignal* oSHIFT; // Перекладка
	CSignal* oSTROBE; // Строб
	CSignal* oRESULT; // Результат

	inline int SignalCount(void)
	{
		return (M.Count());
	}

	inline CSignal* GetSignal(int _index)
	{
		return (M[_index]);
	}
	void AlarmCycleOn(bool _v);
	void SetOnFront(OnFrontDef _OnFront);
	void Latchterminate(AnsiString _msg);
	void SetAlarm(bool _IsAlarm);
};
extern A1730* a1730;
#endif
