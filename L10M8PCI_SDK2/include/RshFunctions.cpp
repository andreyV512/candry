/*!
 * \copyright JSC "Rudnev-Shilyaev"
 * 
 * \file RshFunctions.cpp
 * \date 11.04.2014
 * \version 1.0 [SDK 2.00]
 * 
 * \~english
 * \brief
 * Global functions.
 * 
 * This file contains implementation of common functions used within RSH SDK code.
 * 
 * \~russian
 * \brief
 * Глобальные функции.
 * 
 * В этом файле содержится реализация часто используемых функций, применяемых в RSH SDK.
 * 
 */

#ifndef __RSH_FUNCTIONS_CPP__
#define __RSH_FUNCTIONS_CPP__

#include "RshMacro.h"
#include "RshTypes.h"


void RshTimeReal(RshTime& currentTime)
{
#if defined(RSH_MSWINDOWS)

    SYSTEMTIME stime;   //structure to store system time (in usual time format)
    FILETIME ltime;     //structure to store local time (local time in 64 bits)
    FILETIME ftTimeStamp;

    GetSystemTimeAsFileTime(&ftTimeStamp); // Gets the current system time
    FileTimeToLocalFileTime (&ftTimeStamp,&ltime);//convert in local time and store in ltime
    FileTimeToSystemTime(&ltime,&stime);//convert in system time and store in stime

    currentTime.h = stime.wHour;
    currentTime.m = stime.wMinute;
    currentTime.s = stime.wSecond;
    currentTime.ms = stime.wMilliseconds;
#endif
}

double RshTimeCurrent()
{
#if defined(RSH_MSWINDOWS)
	FILETIME f;
	GetSystemTimeAsFileTime( &f ); // Since 1601 Jan 1
	LARGE_INTEGER t =
	{
		{
			f.dwLowDateTime,
			f.dwHighDateTime
		}
	};
	return t.QuadPart / 10000000.0;  // 100 nano-second intervals
#elif defined(RSH_LINUX)
	struct timeval tv;
	if (gettimeofday( &tv, NULL )) return 0.0;  // Since 1970 Jan 1

	return tv.tv_sec + tv.tv_usec / 1000000.0;
#endif
}

void RshTimeElapsed(double timeSpent, U32& hours, U32& minutes, U32& seconds, U32& milliseconds, U32& microseconds)
{
	hours			= (U32) timeSpent	/	(60 * 60);
	minutes			= (U32) timeSpent	/	60;
    seconds			= (U32) timeSpent	%	60;
    milliseconds	= (U32)(timeSpent * 1000) % 1000;
	microseconds 	= (U32)(timeSpent * 1000 * 1000) % 1000;
}

void RshTimeElapsedString(double timeSpent, std::string& str)
{
	U32 hours, minutes, seconds, milliseconds, microseconds;
	RshTimeElapsed(timeSpent, hours, minutes, seconds, milliseconds, microseconds);

	str = "";
	str += "h: " + RSH_TOSTRING( hours ) + ";";
	str += "min: " + RSH_TOSTRING( minutes ) + ";";
	str += " sec: " + RSH_TOSTRING( seconds ) + ";";
	str += " msec: " + RSH_TOSTRING( milliseconds ) + ";";
	str += " usec: " + RSH_TOSTRING( microseconds );
}

void RshTimeElapsedWString(double timeSpent, std::wstring& str)
{
	U32  hours, minutes, seconds, milliseconds, microseconds;
	RshTimeElapsed(timeSpent, hours, minutes, seconds, milliseconds, microseconds);

	str = L"";
	str += L"h: " + RSH_TOWSTRING( hours ) + L";";
	str += L"min: " + RSH_TOWSTRING( minutes ) + L";";
	str += L" sec: " + RSH_TOWSTRING( seconds ) + L";";
	str += L" msec: " + RSH_TOWSTRING( milliseconds ) + L";";
	str += L" usec: " + RSH_TOWSTRING( microseconds );
}

U64 RshMix() // tries to get unique number
{
	U64 a, b, c;
//	a = clock(), b = time(NULL), c = __rshgetpid();
	a = GetTickCount(), b = time(NULL), c = __rshgetpid();

    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
	a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

U64 RshMix(U64 someNumber) // tries to get unique number
{
	U64 a, b, c;
	a = someNumber, b = time(NULL), c = __rshgetpid();

    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

U32 RshCRC32(U8 *buf, size_t len)
{
    U32 crc_table[256];
    U32 crc; S32 i, j;
 
    for (i = 0; i < 256; i++)
    {
        crc = i;
        for (j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
 
        crc_table[i] = crc;
    };
 
    crc = 0xFFFFFFFFUL;
 
    while (len--) 
        crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
 
    return crc ^ 0xFFFFFFFFUL;
}

double RshRoundD(double val, double pre) // round double value
{

	int sign = ( val >= 0 ) ? 1 : -1;

	val *= sign;

	pre = pow( 10.0, pre );
	val = ((U64)( val * pre + 0.5 )) / pre;

	val *= sign;

	return val;
}


double RshLsbToVoltCoef(U32 gain, double range, U8 bitSize)
{
	return (((U64)1<<bitSize)/range) * gain;
}
double RshVoltToLsbCoef(U32 gain, double range, U8 bitSize)
{
	return (range / gain) / ((U64)1<<bitSize);
}
double RshVoltToLsb(double voltage, U32 gain, double range, U8 bitSize)
{
	return voltage / RshVoltToLsbCoef(gain,range,bitSize);
}
#if defined(RSH_MSWINDOWS)
const wchar_t* RshConvertToUTF16(const char* pStr) {
		static wchar_t szBuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, pStr, -1, szBuf, sizeof(szBuf));
		return szBuf;
}
#elif defined(RSH_LINUX)
const wchar_t* RshConvertToUTF16(const char* pStr) {
                std::string tmp(pStr);
		static std::wstring szBuf;
                szBuf = std::wstring(tmp.begin(),tmp.end());
                //szBuf = std::wstring((wchar_t*)pStr);
             
		return szBuf.c_str();
}
#endif

U32 RshCopyStringTo(const char* str, RshBaseType* userBuffer)
{
	switch( userBuffer->_type )
	{
	case rshBufferTypeU8:
	case rshBufferTypeS8:
		((RSH_BUFFER_S8*)userBuffer)->Copy((S8*)str, strlen(str), strlen(str));
		break;
	
	default:
		return RSH_API_PARAMETER_NOTSUPPORTED;		
	}

	return RSH_API_SUCCESS;
}

#endif //__RSH_FUNCTIONS_CPP__
