//---------------------------------------------------------------------------

#ifndef InverterH
#define InverterH
#include "MitCome/uCInv.h"
#include "vcl.h"
#include <IniFiles.hpp>

//------------------------------------------------------------------------------
class rotationThread;
class Inverter
{
	public:
		Inverter(TIniFile* _ini);
		~Inverter();
		//! @brief Устанавливает скорость в один из параметров
		//! @param number - номер пункта в частотнике ( 4,5,6 )
		//! @param value  - значение в Гц
		bool setParameterSpeed(int number, int value);
		//! @param number номер пункта пч (4,5,6)
		int getParameterSpeed(int number);
		//! Включает вращение
		bool startRotation();
		//! Останавливает вращение
		bool stopRotation();
		//! Считывает все значения, нужно для проверки в отдельном потоке
		bool stateRead();
		//! Переводит ПЧ в режим NET
		bool NETManage();
		//! Сбрасывает ошибки с частотника
		bool ResetErrors();
	private:
		CInv* inverter;
		TCriticalSection* cs;
		bool isConnect;
		//! Поток который будет запрашивать состояние частоника каждые 500мс
		rotationThread *testThread;
		bool mrs;
		void OnProtocol(AnsiString _msg);
};
//------------------------------------------------------------------------------
class rotationThread: public TThread
{
	protected:
		void __fastcall Execute();
	public:
		__fastcall rotationThread(CInv* _inv,TCriticalSection* _cs);
		void Stop();
	private:
		CInv* inverter;
		TCriticalSection* cs;
		bool isStop;
};
extern Inverter *frConverter;
//------------------------------------------------------------------------------

#endif


