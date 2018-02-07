//---------------------------------------------------------------------------

#ifndef LCARD502H
#define LCARD502H
#include "l502api.h"
#include <map.h>
#include <vector.h>
#include <System.hpp>
//#include <Dialogs.hpp>
#pragma comment(lib, "l502api.lib")
//------------------------------------------------------------------------------
using namespace std;
class L502_ProcessThread;
//! Структура настроек для одного логического канала платы LCard502
struct Channel_parameters
{
	Channel_parameters()
	{
		range = 0;
		logicalChannel =0;
		collectedMode = 0;
	}
	//! @brief Входной диапазон
	//! @li 0 Диапазон +/-10V
	//! @li 1 Диапазон +/-5V
	//! @li 2 Диапазон +/-2V
	//! @li 3 Диапазон +/-1V
	//! @li 4 Диапазон +/-0.5V
	//! @li 5 Диапазон +/-0.2V
	uint32_t range;
	//! Номер логического канала 0-31
	uint32_t logicalChannel;
	//! @brief Режим сбора
	//! @li 0 Измерение напряжения относительно общей земли
	//! @li 1 Дифференциальное измерение напряжения
	//! @li 2 Измерение собственного нуля
	uint32_t collectedMode;

};
//! Структура с параметрами АЦП LCard502
struct LCard_parameters
{
	LCard_parameters();
	//! ТаймАут для сбора (с каким периодом будем скидывать данные из ацп в большой буфер,мс)
	int RECV_TOUT;
	//! @brief Режим синхронизации
	//! @li 0 Внутренний сигнал
	//! @li 1 От внешнего мастера по разъему синхронизации
	//! @li 2 По фронту сигнала DI_SYN1
	//! @li 3 По фронту сигнала DI_SYN2
	//! @li 6 По спаду сигнала DI_SYN1
	//! @li 7 По спаду сигнала DI_SYN2
	uint32_t syncMode;
	//! Источник запуска синхронного ввода/вывода см. режимы @link syncMode
	uint32_t syncStartMode;
	//! Частота сбора (Гц.)
	double frequencyCollect;
	//! Частота на канал (Гц.)
	double frequencyPerChannel;
	//! Возвращает кол-во используемых каналов
	int getCountChannels(){return channels.size()+others.size();}
	//! Индивидуальные настройки для каждого канала
	vector< Channel_parameters > channels;
	//! @brief Дополнительные каналы
	//! @li 0 - Ток продольный соленоид
	//! @li 1 - Напряжение продольный соленоид
	//! @li 2 - Ток поперечный соленоид
	//! @li 3 - Напряжение поперечный соленоид
	//! @li 4 - Ток группы прочности
	//! @li 5 - Напряжение группы прочности
	//! @li 6 - Разделение по зонам
	vector< Channel_parameters > others;

};
//! @brief Класс для работы с платой АЦП LCard502
class LCard502
{
	public:
		//! Конструктор
		LCard502();
		//! Деструктор
		~LCard502();
		//! Возвращает значение определенного канала платы АЦП (одиночное измерение)
		vector<double> getFrameValue();

	private:
		//! Возвращает вектор обнаруженных плат
		AnsiString getDeviceList(void);
		//! @brief инициализирует плату АЦП
		//! @param serial_number серийный номер платы,
		//! Cписок номеров в системе можно узнать с помощью ф. @link getDeviceList();
		void Initialization(AnsiString _serial_number);
		bool setSettings();
		map<int,int> voltToPercent;
		//! Хендл платы
		t_l502_hnd handle;
		//! Сколько данных собрали
		__int64 CountCollectedMeasurements;
		//! Вектор данных в вольтах
		vector<double> SourceData;
		//! Настройки платы
		LCard_parameters Parameters;
		AnsiString GetErr(AnsiString _msg,int32_t _err);
		void ErrMsg(AnsiString _msg,int32_t _err);
};
extern LCard502* lcard;
#endif
