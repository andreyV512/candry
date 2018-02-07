/*!
 * \copyright JSC "Rudnev-Shilyaev"
 * 
 * \file RshMacro.h
 * \date 02.04.2014
 * \version 1.0 [SDK 2.00]
 * 
 * \~english
 * \brief
 * Macros and templates used in SDK.
 * 
 * This file contains frequently used code enclosed into macro definitions.
 * 
 * \~russian
 * \brief
 * Макросы и шаблоны, применяемые в SDK.
 * 
 * В этом файле содержатся часто используемые фрагменты кода, оформленные в виде макросов.
 * 
 */

#ifndef __RSH_MACRO_H__
#define __RSH_MACRO_H__

#include "RshTypes.h"

/********************************************\
*       Check Types for Functions            *
\********************************************/
// base types check
#define __TCHECK_RSHU8(X) do{               \
	if( !X ||                               \
	!(X->_type == rshU8) ||    \
	!(X->_typeSize == sizeof(RSH_U8)) )     \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \


#define __TCHECK_RSHU16(X) do{              \
	if( !X ||                               \
	!(X->_type == rshU16) ||   \
	!(X->_typeSize == sizeof(RSH_U16)) )    \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \

#define __TCHECK_RSHU32(X) do{              \
	if( !X ||                               \
	!(X->_type == rshU32) ||   \
	!(X->_typeSize == sizeof(RSH_U32)) )    \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \

#define __TCHECK_RSHU64(X) do{              \
	if( !X ||                               \
	!(X->_type == rshU64) ||   \
	!(X->_typeSize == sizeof(RSH_U64)) )    \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \


#define __TCHECK_RSHS8(X) do{               \
	if( !X ||                               \
	!(X->_type == rshS8) ||    \
	!(X->_typeSize == sizeof(RSH_S8)) )     \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \


#define __TCHECK_RSHS16(X) do{              \
	if( !X ||                               \
	!(X->_type == rshS16) ||   \
	!(X->_typeSize == sizeof(RSH_S16)) )    \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \

#define __TCHECK_RSHS32(X) do{              \
	if( !X ||                               \
	!(X->_type == rshS32) ||   \
	!(X->_typeSize == sizeof(RSH_S32)) )    \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \

#define __TCHECK_RSHS64(X) do{              \
	if( !X ||                               \
	!(X->_type == rshS64) ||   \
	!(X->_typeSize == sizeof(RSH_S64)) )    \
	return RSH_API_PARAMETER_INVALID;       \
						 }while(0);         \


#define __TCHECK_RSHDOUBLE(X) do{            \
	if( !X ||                                \
	!(X->_type == rshDouble) || \
	!(X->_typeSize == sizeof(RSH_DOUBLE)) )  \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \


#define __TCHECK_RSHFLOAT(X) do{              \
	if( !X ||                                  \
	!(X->_type == rshFloat) ||   \
	!(X->_typeSize == sizeof(RSH_FLOAT)) )    \
	return RSH_API_PARAMETER_INVALID;          \
						 }while(0);            \


#define __TCHECK_RSHBOOL(X) do{              \
	if( !X ||                                \
	!(X->_type == rshBool) ||   \
	!(X->_typeSize == sizeof(RSH_BOOL)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \


//  base type pointers check
#define __TCHECK_RSHU8P(X) do{               \
	if( !X ||                                \
	!(X->_type == rshU8P) ||    \
	!(X->_typeSize == sizeof(RSH_U8P)) )     \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \


#define __TCHECK_RSHU16P(X) do{              \
	if( !X ||                                \
	!(X->_type == rshU16P) ||   \
	!(X->_typeSize == sizeof(RSH_U16P)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \

#define __TCHECK_RSHU32P(X) do{              \
	if( !X ||                                \
	!(X->_type == rshU32P) ||   \
	!(X->_typeSize == sizeof(RSH_U32P)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \

#define __TCHECK_RSHU64P(X) do{              \
	if( !X ||                                \
	!(X->_type == rshU64P) ||   \
	!(X->_typeSize == sizeof(RSH_U64P)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \


#define __TCHECK_RSHS8P(X) do{               \
	if( !X ||                                \
	!(X->_type == rshS8P) ||    \
	!(X->_typeSize == sizeof(RSH_S8P)) )     \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \


#define __TCHECK_RSHS16P(X) do{              \
	if( !X ||                                \
	!(X->_type == rshS16P) ||   \
	!(X->_typeSize == sizeof(RSH_S16P)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \

#define __TCHECK_RSHS32P(X) do{              \
	if( !X ||                                \
	!(X->_type == rshS32P) ||   \
	!(X->_typeSize == sizeof(RSH_S32P)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \

#define __TCHECK_RSHS64P(X) do{              \
	if( !X ||                                \
	!(X->_type == rshS64P) ||   \
	!(X->_typeSize == sizeof(RSH_S64P)) )    \
	return RSH_API_PARAMETER_INVALID;        \
						 }while(0);          \


#define __TCHECK_RSHDOUBLEP(X) do{             \
	if( !X ||                                  \
	!(X->_type == rshDoubleP) ||  \
	!(X->_typeSize == sizeof(RSH_DOUBLEP)) )   \
	return RSH_API_PARAMETER_INVALID;          \
						 }while(0);            \

#define __TCHECK_RSHFLOATP(X) do{              \
	if( !X ||                                  \
	!(X->_type == rshFloatP) ||   \
	!(X->_typeSize == sizeof(RSH_FLOATP)) )    \
	return RSH_API_PARAMETER_INVALID;          \
						 }while(0);            \


#define __TCHECK_RSHBOOLP(X) do{               \
	if( !X ||                                  \
	!(X->_type == rshBoolP) ||    \
	!(X->_typeSize == sizeof(RSH_BOOLP)) )     \
	return RSH_API_PARAMETER_INVALID;          \
						 }while(0);            \

#define __TCHECK_RSHU8PP(X) do{               \
	if( !X ||                                 \
	!(X->_type == rshU8PP) ||    \
	!(X->_typeSize == sizeof(RSH_U8PP)) )     \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \


#define __TCHECK_RSHU16PP(X) do{              \
	if( !X ||                                 \
	!(X->_type == rshU16PP) ||   \
	!(X->_typeSize == sizeof(RSH_U16PP)) )    \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \

#define __TCHECK_RSHU32PP(X) do{              \
	if( !X ||                                 \
	!(X->_type == rshU32PP) ||   \
	!(X->_typeSize == sizeof(RSH_U32PP)) )    \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \

#define __TCHECK_RSHU64PP(X) do{              \
	if( !X ||                                 \
	!(X->_type == rshU64PP) ||   \
	!(X->_typeSize == sizeof(RSH_U64PP)) )    \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \


#define __TCHECK_RSHS8PP(X) do{               \
	if( !X ||                                 \
	!(X->_type == rshS8PP) ||    \
	!(X->_typeSize == sizeof(RSH_S8PP)) )     \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \


#define __TCHECK_RSHS16PP(X) do{              \
	if( !X ||                                 \
	!(X->_type == rshS16PP) ||   \
	!(X->_typeSize == sizeof(RSH_S16PP)) )    \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \

#define __TCHECK_RSHS32PP(X) do{              \
	if( !X ||                                 \
	!(X->_type == rshS32PP) ||   \
	!(X->_typeSize == sizeof(RSH_S32PP)) )    \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \

#define __TCHECK_RSHS64PP(X) do{              \
	if( !X ||                                 \
	!(X->_type == rshS64PP) ||   \
	!(X->_typeSize == sizeof(RSH_S64PP)) )    \
	return RSH_API_PARAMETER_INVALID;         \
						 }while(0);           \


#define __TCHECK_RSHDOUBLEPP(X) do{             \
	if( !X ||                                   \
	!(X->_type == rshDoublePP) ||  \
	!(X->_typeSize == sizeof(RSH_DOUBLEPP)) )   \
	return RSH_API_PARAMETER_INVALID;           \
						 }while(0);             \

#define __TCHECK_RSHFLOATPP(X) do{              \
	if( !X ||                                   \
	!(X->_type == rshFloatPP) ||   \
	!(X->_typeSize == sizeof(RSH_FLOATPP)) )    \
	return RSH_API_PARAMETER_INVALID;           \
						 }while(0);             \


#define __TCHECK_RSHBOOLPP(X) do{               \
	if( !X ||                                   \
	!(X->_type == rshBoolPP) ||    \
	!(X->_typeSize == sizeof(RSH_BOOLPP)) )     \
	return RSH_API_PARAMETER_INVALID;           \
						 }while(0);             \

// other types check
#define __TCHECK_RSHREGISTER(X) do{            \
	if( !X ||                                  \
	!(X->_type == rshRegister) || \
	!(X->_typeSize == sizeof(RshRegister)) )   \
	return RSH_API_PARAMETER_INVALID;          \
						 }while(0);            \

#define __TCHECK_RSHBUFFERDEVICEFULLINFO(X) do{                  \
	if( !X ||                                                    \
	!(X->_type == rshBufferTypeDeviceFullInfo) ||   \
	!(X->_typeSize == sizeof(RSH_BUFFER_DEVICE_FULL_INFO)) )     \
	return RSH_API_PARAMETER_INVALID;                            \
						 }while(0);                              \

#define __TCHECK_RSHONBOARDBASEINFO(X) do{              \
	if( !X ||                                           \
	!(X->_type == rshOnboardBaseInfo) ||   \
	!(X->_typeSize == sizeof(RshOnboardBaseInfo)) )     \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \

#define __TCHECK_RSHBUFFERDEVICEBASEINFO(X) do{				     \
	if( !X ||													 \
	!(X->_type == rshBufferTypeDeviceBaseInfo) ||   \
	!(X->_typeSize == sizeof(RSH_BUFFER_DEVICE_BASE_INFO)) )	 \
	return RSH_API_PARAMETER_INVALID;							 \
						}while(0);								 \




#define __TCHECK_RSHDEVICEBASEINFO(X) do{              \
	if( !X ||                                          \
	!(X->_type == rshDeviceBaseInfo) ||   \
	!(X->_typeSize == sizeof(RshDeviceBaseInfo)) )     \
	return RSH_API_PARAMETER_INVALID;                  \
						}while(0);                     \


#define __TCHECK_RSHDEVICEBUFFER(X) do{                 \
	if( !X ||                                           \
	!(X->_type == rshDeviceBuffer) ||      \
	!(X->_typeSize == sizeof(RshDeviceBuffer)) )        \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \


#define __TCHECK_RSHBOARDPORTINFO(X) do{                \
	if( !X ||                                           \
	!(X->_type == rshBoardPortInfo) ||     \
	!(X->_typeSize == sizeof(RshBoardPortInfo)) )       \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \

#define __TCHECK_RSHBUFFERTYPES8(X) do{                 \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeS8) ||      \
	!(X->_typeSize == sizeof(RSH_BUFFER_S8)) )          \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \

#define __TCHECK_RSHBUFFERTYPEU8(X) do{                 \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeU8) ||      \
	!(X->_typeSize == sizeof(RSH_BUFFER_U8)) )          \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \

#define __TCHECK_RSHBUFFERTYPEU8P(X) do{                \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeU8P) ||     \
	!(X->_typeSize == sizeof(RSH_BUFFER_U8P)) )         \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \

#define __TCHECK_RSHBUFFERTYPES16(X) do{                \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeS16) ||     \
	!(X->_typeSize == sizeof(RSH_BUFFER_S16)) )         \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \


#define __TCHECK_RSHBUFFERTYPEU16(X) do{                \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeU16) ||     \
	!(X->_typeSize == sizeof(RSH_BUFFER_U16)) )         \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \


#define __TCHECK_RSHBUFFERTYPEU32(X) do{                \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeU32) ||     \
	!(X->_typeSize == sizeof(RSH_BUFFER_U32)) )         \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \


#define __TCHECK_RSHBUFFERTYPES32(X) do{                \
    if( !X ||                                           \
    !(X->_type == rshBufferTypeS32) ||     \
    !(X->_typeSize == sizeof(RSH_BUFFER_S32)) )         \
    return RSH_API_PARAMETER_INVALID;                   \
                        }while(0);                      \

#define __TCHECK_RSHBUFFERTYPEDOUBLE(X) do{             \
	if( !X ||                                           \
	!(X->_type == rshBufferTypeDouble) ||  \
	!(X->_typeSize == sizeof(RSH_BUFFER_DOUBLE)) )      \
	return RSH_API_PARAMETER_INVALID;                   \
						}while(0);                      \
						

#define __TCHECK_RSHBUFFERTYPECHANVOLTAGE(X) do{                \
	if( !X ||                                                   \
	!(X->_type == rshBufferTypeChannelVoltage) ||  \
	!(X->_typeSize == sizeof(RSH_BUFFER_CHANVOLTAGE)) )         \
	return RSH_API_PARAMETER_INVALID;                           \
						}while(0);                              \


#define __TCHECK_RSHBUFFERTYPECALIBRATIONITEM(X) do{            \
	if( !X ||                                                   \
	!(X->_type == rshBufferTypeCalibrationItem) ||              \
	!(X->_typeSize == sizeof(RSH_BUFFER_CALIBRATIONITEM)) )     \
	return RSH_API_PARAMETER_INVALID;                           \
						}while(0);                              \


#define __TCHECK_RSHBUFFERTYPECALIBRATIONGROUP(X) do{           \
	if( !X ||                                                   \
	!(X->_type == rshBufferTypeCalibrationGroup) ||             \
	!(X->_typeSize == sizeof(RSH_BUFFER_CALIBRATIONGROUP)) )    \
	return RSH_API_PARAMETER_INVALID;                           \
						}while(0);                              \


#define __TCHECK_RSHFRDACTUNE(X) do{              \
	if( !X ||                                     \
	!(X->_type == rshFRDACTune) ||   \
	!(X->_typeSize == sizeof(RshFRDACTune)) )     \
	return RSH_API_PARAMETER_INVALID;             \
						 }while(0);               \


#define __TCHECK_RSHCALIBRATIONINFO(X) do{              \
	if( !X ||                                           \
	!(X->_type == rshCalibrationInfo) ||   \
	!(X->_typeSize == sizeof(RshCalibrationInfo)) )     \
	return RSH_API_PARAMETER_INVALID;                   \
						 }while(0);                     \


#define __TCHECK_RSHSETTINGINFO(X) do{                    \
	if( !X ||                                             \
	!(X->_type == rshSettingInfo)         || \
	!(X->_typeSize == sizeof(RshSettingInfo)) )           \
	return RSH_API_PARAMETER_INVALID;                     \
						 }while(0);                       \



#define __TCHECK_RSHSETTING(X) do{                     \
	if( !X ||                                          \
	!(X->_type == rshSetting)         ||  \
	!(X->_typeSize == sizeof(RshSetting)) )            \
	return RSH_API_PARAMETER_INVALID;                  \
						 }while(0);                    \


#define __TCHECK_RSHINITPACKET(X) do{                  \
	if( !X ||                                          \
	!(X->_type == rshInitPacket)      ||  \
	!(X->_typeSize == sizeof(RshInitPacket)) )         \
	return RSH_API_PARAMETER_INVALID;                  \
						 }while(0);                    \


#define __TCHECK_RSHCALIBRATIONCONTROL(X) do{            \
	if( !X ||                                            \
	!(X->_type == rshCalibrationControl) || \
	!(X->_typeSize == sizeof(RshCalibrationControl)) )   \
	return RSH_API_PARAMETER_INVALID;                    \
						 }while(0);                      \

#define __TCHECK_RSHCALIBRATIONPARAMETERS(X) do{            \
	if( !X ||												\
	!(X->_type == rshCalibrationParameters) || \
	!(X->_typeSize == sizeof(RshCalibrationParameters)) )   \
	return RSH_API_PARAMETER_INVALID;						\
						 }while(0);							\


#define __TCHECK_RSHCALIBRATIONITEMENTITY(X) do{            \
	if( !X ||												\
	!(X->_type == rshCalibrationItemEntity) ||              \
	!(X->_typeSize == sizeof(RshCalibrationItemEntity)) )   \
	return RSH_API_PARAMETER_INVALID;						\
						 }while(0);							\


#define __TCHECK_RSHCALIBRATIONITEMREGISTER(X) do{           \
	if( !X ||												 \
	!(X->_type == rshCalibrationItemRegister) ||             \
	!(X->_typeSize == sizeof(RshCalibrationItemRegister)) )  \
	return RSH_API_PARAMETER_INVALID;						 \
						 }while(0);							 \

#define __TCHECK_RSHCALIBRATIONITEMREGOFFSET(X) do{          \
	if( !X ||												 \
	!(X->_type == rshCalibrationItemRegOffset) ||            \
	!(X->_typeSize == sizeof(RshCalibrationItemRegOffset)) ) \
	return RSH_API_PARAMETER_INVALID;						 \
						 }while(0);							 \

#define __TCHECK_RSHCALIBRATIONITEMBUTTON(X) do{             \
	if( !X ||												 \
	!(X->_type == rshCalibrationItemButton) ||               \
	!(X->_typeSize == sizeof(RshCalibrationItemButton)) )    \
	return RSH_API_PARAMETER_INVALID;						 \
						 }while(0);							 \

#define __TCHECK_RSHCALIBRATIONITEMFILEPATH(X) do{           \
	if( !X ||												 \
	!(X->_type == rshCalibrationItemFilePath) ||             \
	!(X->_typeSize == sizeof(RshCalibrationItemFilePath)) )  \
	return RSH_API_PARAMETER_INVALID;						 \
						 }while(0);							 \


#define __TCHECK_RSHIDEBUG(X) do{                       \
	if( !X ||                                           \
	!(X->_type == rshIDebug) ||            \
	!(X->_typeSize == sizeof(RshIDebug)) )              \
	return RSH_API_PARAMETER_INVALID;                   \
						 }while(0);                     \


#define __TCHECK_RSHDEVICEKEY(X) do{                    \
	if( !X ||                                           \
	!(X->_type == rshDeviceKey) ||         \
	!(X->_typeSize == sizeof(RshDeviceKey)) )           \
	return RSH_API_PARAMETER_INVALID;                   \
						 }while(0);                     \


#define __TCHECK_RSHCALIBRATIONCHANNELPARAMETER(X) do{            \
	if( !X ||                                                     \
	!(X->_type == rshCalibrationChannelParameter) ||              \
	!(X->_typeSize == sizeof(RshCalibrationChannelParameter)) )   \
	return RSH_API_PARAMETER_INVALID;                             \
						 }while(0);                               \

/********************************************\
*               Templates                    *
\********************************************/

#define UNIC_VAR(varName,lineNumber) varName##lineNumber
#define forRSH(_rshBufferTypeVariable_) for(bool UNIC_VAR(iterator_valid,__LINE__)=_rshBufferTypeVariable_.SetItToTheFirstElement(); \
            UNIC_VAR(iterator_valid,__LINE__);UNIC_VAR(iterator_valid,__LINE__)=_rshBufferTypeVariable_.SetItToTheNextElement())

#define RSH_DIM(a)    (sizeof(a) / sizeof(a[0]))

#define RSH_CMPDOUBLE(a,b) (fabs(a - b) <  std::numeric_limits<double>::epsilon())

#define RSH_TOSTRING( x ) dynamic_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()

#define RSH_TOWSTRING( x ) dynamic_cast< std::wstringstream & >( \
            ( std::wstringstream() << std::dec << x ) ).str()

template <class T> inline T    CutV(T v, T mi, T ma)
{
	if(v < mi) v = mi; else if(v > ma) v = ma; return v;
}
/*......................................................................*/
template <class T> inline void Cut(T& v, T mi, T ma)
{
	if(v < mi) v = mi; else if(v > ma) v = ma;
}
/*----------------------------------------------------------------------*/
template <class T> inline void MiMa(T v, T& mi, T& ma)
{
	if(v < mi) mi = v; else if(v > ma) ma = v;
}
/*----------------------------------------------------------------------*/
template <class T> inline void Swap(T& v1, T& v2)
{
	T t = v1; v1 = v2; v2 = t;
};

/*----------------------------------------------------------------------*/
# define rshUTF16(str)  RshConvertToUTF16(str)

#endif  //__RSH_MACRO_H__
