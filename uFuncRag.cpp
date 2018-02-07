//---------------------------------------------------------------------------
#pragma hdrstop
#include "uFuncRag.h"
#include <Forms.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
void CheckThread(AnsiString _head)
{
	WideString w=_head;
	AnsiString msg="Thread: ";
	msg+=GetCurrentThreadId();
	WideString wmsg=msg;
	Application->MessageBoxW(wmsg.c_bstr(),w.c_bstr(),MB_ICONWARNING);
}
