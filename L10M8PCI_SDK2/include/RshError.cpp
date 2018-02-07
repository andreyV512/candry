/*!
 * \copyright JSC "Rudnev-Shilyaev"
 * 
 * \file RshError.cpp
 * \date 02.12.2013
 * \version 1.0 [SDK 2.00]
 * 
 * \~english
 * \brief
 * Error process class and data structures connected.
 * 
 * \~russian
 * \brief
 * Класс для формирования сообщений об ошибках и связанные структуры данных.
 * 
 */

#ifndef __RSHERROR_CPP__
#define __RSHERROR_CPP__

#include "RshError.h"
#include "RshErrorDescription.h"


U32 RshError::GetSystemErrorDescriptionUTF16(U32 error, wchar_t* str, U32 maxLength)
{
	if(!str)
		return RSH_API_PARAMETER_ZEROADDRESS;

	if(maxLength == 0)
		return RSH_API_PARAMETER_INVALID;

	std::wstring errorMessage(L"");

#if defined(RSH_MSWINDOWS)
        U32 winApiCode = error & 0xffff;
		wchar_t* err;		
		
		U32 len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           winApiCode,
						   0, // default language
                           (LPTSTR) &err,
                           0,
                           NULL);
		if( len == 0)
		{
			return RSH_API_FUNCTION_ERRORHAPPEND|GetLastError();
		}
		errorMessage = std::wstring(err);
#elif defined(RSH_LINUX)
       char* errormsg = strerror(error & 0xffff);
       std::string tmp = std::string(errormsg);
       errorMessage = std::wstring(tmp.begin(),tmp.end());   
#endif

		U32 copySize = errorMessage.length()>maxLength-1 ? maxLength-1: errorMessage.length();

		try
		{
			memcpy(str,errorMessage.c_str(), copySize*sizeof(wchar_t));
			str[copySize] = 0;
		}
		catch(...)
		{
			return RSH_API_MEMORY_COPYERROR;
		}

	return RSH_API_SUCCESS;
}

//------------------------------------------------------------------------------------------------
U32 RshError::GetSystemErrorDescription(U32 error, char* str, U32 maxLength)
{
	if(!str)
		return RSH_API_PARAMETER_ZEROADDRESS;

	if(maxLength == 0)
		return RSH_API_PARAMETER_INVALID;

	wchar_t* buf = new wchar_t[maxLength];
	if(!buf)
		return RSH_API_MEMORY_ALLOCATIONERROR;

	U32 res = GetSystemErrorDescriptionUTF16(error, buf, maxLength);
	if(res != RSH_API_SUCCESS)
		return res;	

#if defined(RSH_MSWINDOWS)
    int err = WideCharToMultiByte(CP_ACP,0,buf,-1,str,maxLength,NULL,NULL);
	if(err == 0)
		return RSH_API_FUNCTION_ERRORHAPPEND |GetLastError();
#elif defined(RSH_LINUX)	
	std::wstring errorUTF16 = std::wstring(buf);
	std::string errorMessage = std::string(errorUTF16.begin(),errorUTF16.end());  

	U32 copySize = errorMessage.length()>maxLength-1 ? maxLength-1: errorMessage.length();

	try
	{
		memcpy(str,errorMessage.c_str(), copySize*sizeof(char));
		str[copySize] = 0;
	}
	catch(...)
	{
		return RSH_API_MEMORY_COPYERROR;
	}
      
#endif

	return RSH_API_SUCCESS;
}
//------------------------------------------------------------------------------------------------
U32 RshError::GetErrorDescriptionUTF16(U32 error, wchar_t* str, U32 maxLength, RSH_LANGUAGE language)
{
	if(!str)
		return RSH_API_PARAMETER_ZEROADDRESS;

	if(maxLength == 0)
		return RSH_API_PARAMETER_INVALID;	
	
	U32 errorCodeRsh = error & 0xffff0000;

	std::wstring errorMessage = L"";

	int i=0;
	for(i=0;i<RSH_DIM(RshErrorList);++i)
	{
		if(RshErrorList[i].ecode == errorCodeRsh)
			break;
	}

	if(i<RSH_DIM(RshErrorList))
	{
		switch(language)
		{
			case RSH_LANGUAGE_RUSSIAN:
				errorMessage = std::wstring(RshErrorList[i].ru);
				break;
			case RSH_LANGUAGE_ENGLISH:
			default:
				errorMessage = std::wstring(RshErrorList[i].en);
				break;
		}	
		
	}
	else
	{
		std::wstringstream streamVal;
		streamVal << std::hex << (error & ~0xffff);
		std::wstring rstr;			

		if( language == RSH_LANGUAGE_ENGLISH )
			errorMessage = std::wstring(rshUTF16("Error not found. Error code:  0x")) +  std::wstring(streamVal.str());
		else
			errorMessage = std::wstring(rshUTF16("Неизвестная ошибка. Код ошибки:  0x")) +  std::wstring(streamVal.str());
	}

	U32 copySize = errorMessage.length()>maxLength-1 ? maxLength-1: errorMessage.length();

	try
	{
		memcpy(str,errorMessage.c_str(), copySize*sizeof(wchar_t));
		str[copySize] = 0;
	}
	catch(...)
	{
		return RSH_API_MEMORY_COPYERROR;
	}

	return RSH_API_SUCCESS;
}	

//------------------------------------------------------------------------------------------------
U32 RshError::GetErrorDescription(U32 error, char* str, U32 maxLength, RSH_LANGUAGE language)
{
	if(!str)
		return RSH_API_PARAMETER_ZEROADDRESS;

	if(maxLength == 0)
		return RSH_API_PARAMETER_INVALID;

	wchar_t* buf = new wchar_t[maxLength];
	if(!buf)
		return RSH_API_MEMORY_ALLOCATIONERROR;

	U32 res = GetErrorDescriptionUTF16(error, buf, maxLength, language);
	if(res != RSH_API_SUCCESS)
		return res;	

#if defined(RSH_MSWINDOWS)
    int err = WideCharToMultiByte(CP_ACP,0,buf,-1,str,maxLength,NULL,NULL);
	if(err == 0)
		return RSH_API_FUNCTION_ERRORHAPPEND |GetLastError();
#elif defined(RSH_LINUX)	
	std::wstring errorUTF16 = std::wstring(buf);
	std::string errorMessage = std::string(errorUTF16.begin(),errorUTF16.end());  

	U32 copySize = errorMessage.length()>maxLength-1 ? maxLength-1: errorMessage.length();

	try
	{
		memcpy(str,errorMessage.c_str(), copySize*sizeof(char));
		str[copySize] = 0;
	}
	catch(...)
	{
		return RSH_API_MEMORY_COPYERROR;
	}
      
#endif

	return RSH_API_SUCCESS;
}
//------------------------------------------------------------------------------------------------
void RshError::PrintError(U32 error, RSH_LANGUAGE language, bool includeSystemErrorInfo)
{
		#if defined(RSH_MSWINDOWS)
		 _wsetlocale( LC_ALL, L"Russian" );
		#elif defined(RSH_LINUX)
          setlocale( LC_ALL, "ru_RU.utf8" );
		#endif

		if(error != RSH_API_SUCCESS)
		{
			if(language == RSH_LANGUAGE_RUSSIAN)
				std::wcout<< rshUTF16("--ОШИБКА[0x");
			else 
				std::wcout<<std::endl<< rshUTF16("--ERROR[0x");

			wchar_t bufferRshApi[4096];
            U32 res = RshError::GetErrorDescriptionUTF16(error & ~0xffff, bufferRshApi,4096,language);
			if(res == RSH_API_SUCCESS)
			{
				std::wcout << std::hex
					<< (error & 0xffff0000) << rshUTF16("]-- ")
					<< bufferRshApi<<std::endl ;
			}
			else
			{
				std::wcout <<rshUTF16("Couldn't format error message - GetErrorDescriptionUTF16 failed with code 0x")<<std::hex<<res;
			}
			
			if(includeSystemErrorInfo && (error & 0xffff) )
			{
				wchar_t buffer[4096];

                U32 res = RshError::GetSystemErrorDescriptionUTF16( error & 0xffff, buffer, 4096 );
				if(res == RSH_API_SUCCESS)
				{

					if(language == RSH_LANGUAGE_RUSSIAN)
						std::wcout<<std::endl<<rshUTF16("Системная ошибка [");
					else
						std::wcout<<std::endl<<rshUTF16("System error [");

					std::wcout<<std::dec<< (error & 0xffff)<<rshUTF16("]: ");                          
									std::wcout<< buffer<<std::endl;
				}
				else
				{
					std::wcout <<rshUTF16("Couldn't format error message - GetSystemErrorDescriptionUTF16 failed with code 0x")<<std::hex<<res;
				}
			}
			
		}
		else
		{
			if(language == RSH_LANGUAGE_RUSSIAN)
				std::wcout<<std::endl<< rshUTF16("--УСПЕХ--")<<std::endl;
			else 
				std::wcout<<std::endl<< rshUTF16("--SUCCESS--")<<std::endl;		
		}
}

//------------------------------------------------------------------------------------------------
void RshError::PrintErrorDebug(U32 error, wchar_t* message, bool includeSystemErrorInfo)
{
		
		#if defined(RSH_MSWINDOWS)
		_wsetlocale( LC_ALL, L"Russian" );	
		#elif defined(RSH_LINUX)
          setlocale( LC_ALL, "ru_RU.utf8" );
		#endif
		RSH_LANGUAGE language = RSH_LANGUAGE_ENGLISH;

		if(error != RSH_API_SUCCESS)
		{
			
			std::wcout<<std::endl<<rshUTF16("--ERROR[0x");

			wchar_t bufferRshApi[4096];
            U32 res = RshError::GetErrorDescriptionUTF16(error & ~0xffff, bufferRshApi,4096,language);
			if(res == RSH_API_SUCCESS)
			{
				std::wcout << std::hex
				<< (error & 0xffff0000) << rshUTF16("]-- ");
				if(message)
					std::wcout<<message;
				else
					std::wcout<<rshUTF16("~~!wchar_t* message points to NULL!~~");
				std::wcout<<rshUTF16(": ")<< bufferRshApi<<std::endl ;
			}
			else
			{
				std::wcout <<rshUTF16("Couldn't format error message - GetErrorDescriptionUTF16 failed with code 0x")<<std::hex<<res;
			}

			
			
			if(includeSystemErrorInfo && (error & 0xffff) )
			{
				wchar_t buffer[4096];

                U32 res = RshError::GetSystemErrorDescriptionUTF16( error & 0xffff, buffer, 4096 );
				if(res == RSH_API_SUCCESS)
				{
					std::wcout<<std::endl<<rshUTF16("System error [");
					std::wcout<<std::dec<< (error & 0xffff)<<rshUTF16("]: ");                          
                                std::wcout<< buffer<<std::endl;
				}
				else
				{
					std::wcout <<rshUTF16("Couldn't format error message - GetSystemErrorDescriptionUTF16 failed with code 0x")<<std::hex<<res;
				}
			}
			
		}
		else
		{
			std::wcout<<std::endl<< rshUTF16("--SUCCESS-- ")<<message<<std::endl;		
		}
}
/*
//------------------------------------------------------------------------------------------------
RshError::~RshError()
{ 
	
}
//------------------------------------------------------------------------------------------------
RshError::RshError()
{	

}
*/
#include "RshFunctions.cpp"

#endif //__RSHERROR_CPP__

