// ---------------------------------------------------------------------------
#ifndef SignalH
#define SignalH
#include<System.hpp>
#include<SyncObjs.hpp>

// ---------------------------------------------------------------------------
class CSignal
{
	friend class A1730;
	friend class A1730_virtual;

private:
	typedef void(__closure * tagOnSet)(void);

	typedef AnsiString (__closure * tagOnWait)(bool _value, CSignal * _signal,
		DWORD _tm);

	CSignal(AnsiString _name, bool _in, int _index, tagOnSet _OnSet,
		TCriticalSection* _cs, tagOnWait _OnWait);

	bool value;
	bool value_prev;
	AnsiString name;
	bool in;
	int index;
	tagOnSet OnSet;
	tagOnWait OnWait;
	TCriticalSection* cs;
	TEvent* event;
	DWORD last_changed;

	bool WasConst0(bool _value, DWORD _period);
	void Set0(bool _value);

public:
	~CSignal();
	bool Get(void);
	bool WasConst(bool _value, DWORD _period);
	void Set(bool _value);
	AnsiString Wait(bool _value, DWORD _tm);

	inline bool GetIn()
	{
		return (in);
	}

	inline int GetIndex()
	{
		return (index);
	}

	inline AnsiString GetName()
	{
		return (name);
	}
};
#endif
