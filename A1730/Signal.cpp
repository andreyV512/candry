// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Signal.h"
#include<SysUtils.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

CSignal::CSignal(AnsiString _name, bool _in, int _index, tagOnSet _OnSet,
	TCriticalSection* _cs, tagOnWait _OnWait)
{
	name = _name;
	in = _in;
	index = _index;
	value = false;
	value_prev = false;
	OnSet = _OnSet;
	OnWait = _OnWait;
	cs = _cs;
	if (in)
		event = new TEvent(true);
	else
		event = NULL;
	last_changed = GetTickCount();
}

CSignal::~CSignal()
{
	if (event != NULL)
		delete event;
}

bool CSignal::Get(void)
{
	bool ret;
	cs->Enter();
	ret = value;
	cs->Leave();
	return (ret);
}

bool CSignal::WasConst(bool _value, DWORD _period)
{
	bool ret;
	cs->Enter();
	ret = value;
	DWORD last_changed1 = last_changed;
	cs->Leave();
	if (last_changed1 <= GetTickCount() - _period)
	{
		if (ret == _value)
			return (true);
	}
	return (false);
}

bool CSignal::WasConst0(bool _value, DWORD _period)
{
	if (last_changed <= GetTickCount() - _period)
	{
		if (value == _value)
			return (true);
	}
	return (false);
}

void CSignal::Set0(bool _value)
{
	if (in)
	{
		AnsiString a = "Signal::Set: попытка выставить входной сигнал: ";
		a += name;
		throw(Exception(a));
	}
	if (value_prev != value)
		last_changed = GetTickCount();
	value_prev = value;
	value = _value;
	last_changed = GetTickCount();
	OnSet();
}

void CSignal::Set(bool _value)
{
	cs->Enter();
	{
		if (in)
		{
			AnsiString a = "Signal::Set: попытка выставить входной сигнал: ";
			a += name;
			throw(Exception(a));
		}
		if (value_prev != value)
			last_changed = GetTickCount();
		value_prev = value;
		value = _value;
		last_changed = GetTickCount();
	} cs->Leave();
	OnSet();
}

AnsiString CSignal::Wait(bool _value, DWORD _tm)
{
	if (!in)
	{
		AnsiString a = "Signal::Wait: попытка подождать выходной сигнал: ";
		a += name;
		throw(Exception(a));
	}
	if (Get() == _value)
		return ("Ok");
	return (OnWait(_value, this, _tm));
}
