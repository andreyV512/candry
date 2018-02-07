// ---------------------------------------------------------------------------

#ifndef rudshelH
#define rudshelH

#include <vector>
#include <stdint.h>
#include <windows.h>

#include <IADCDevice.h>
#include <IFactory.h>
#include <DllClient.h>
#include <REGKEY\VT_Drivers.ri>
#include <REGKEY\VT_Library.ri>
#include <ADC_Errors.h>
#include "tubefile.h"

#define DEVICE_NAME_DEFAULT     _T("LAN10M8PCI")    	// Название драйвера
// #define BASE_ADDRESS_DEFAULT    1	           			// Адрес платы
#define DRQ_NUMBER_DEFAULT      1               		// Номер канала ПДП
#define IRQ_NUMBER_DEFAULT      DRQ_NUMBER_DEFAULT    	// Номер прерывания

#define RUDSHEL_WAIT_TIMEOUT			10000			  	// в мс
#define MAX_BOARD_COUNT		(2)

enum RudShelSyncType
{
	SYNC_DECLINE = 0, SYNC_FRONT = 1
};

// ! Структура, описывающая параметры сбора платой Руднев-Шиляев LAn10M8PCI
struct acquireSettings
{
	// ! Длина одного измерения (например, 484, 996 и т.д.)
	int32_t measureSize;
	// ! Количество датчиков
	int32_t sensorCount;
	// ! Частота дискретизации
	float quantFrequency;
	// ! Уровень синхронизации
	int32_t syncLevel;
	// ! Коэффициент усиления
	int32_t ampGain;
	// ! Число стробов в 1 пакете
	int32_t strobesPerPacket;
	// ! Задержка между приходом синхроимпульса и собственно
	// ! началом измерений. Фактически первые startDelay отсчётов
	// ! АЦП пропускаются измерителньой платой Руднев-Шиляев
	int32_t startDelay;
	// ! Вид синхронизации
	RudShelSyncType syncType;
};

// ! Класс по работе с платой АЦП Руднев-Шиляев LAn10M8PCI
class _LAN10M8PCI
{
private:
	// ! Размер собранных данных (размер отсчетов в измерении * число измерений в пакете * число датчиков )
	int32_t bufferSize;
	// ! @brief Частота сбора
	// ! @deprecated Нигде не используется, сделано как защита от неправильной инициализации
	float quantFreq;
	// ! Диапазон измеряемых напряжений
	double voltRange;
	// ! Максимальное усиление, которое поддерживает плата РудШел
	int32_t intRange;
	// ! Флаг открыта ли плата
	bool opened;

	// ! Событие для реагирования на прерывание
	HANDLE hIRQEvent;
	// ! Объект, обладающий набором методов для загрузки драйвера в память
	DllClient LAxClient;
	// ! Указатель на объект драйвера
	IFactory* LAxFactory;
	// ! Указатель на объект интерфейса
	IADCDevice* pADC;

	int32_t errorCode;

	short* data;
	void pr(AnsiString _msg);

public:
	// ! Конструктор
	_LAN10M8PCI();
	// ! Деструктор
	~_LAN10M8PCI();

	// ! Открывает плату для сбора - загружает драйвер, получает интерфейс для работы
	// ! @param baseAddress Адрес платы РудШел, нумеруются по порядку 0,1
	bool checkBoard(const int& baseAddress);
	// ! @brief Задает настройки сбора данных, подгружает константы для работы
	// ! @param Передается структура @link acquireSettings @endlink
	bool initSettings(const acquireSettings& settings);
	// ! Основная функция - начать сбор параллельный данных
	short* yield();

	// ! Возвращает код последней ошибки
	inline int32_t lastError()
	{
		return errorCode;
	}

	// ! Возвращает диапазон измеряемых напряжений платы (см. ADC_GET_RANGE_BIPOLAR в документации РудШел)
	inline double voltageRange()
	{
		return voltRange;
	}

	// ! Возвращает максимальное усиление платы (см. ADC_GET_MAXAMP в документации РудШел)
	inline int32_t adcRange()
	{
		return intRange;
	}

	// ! Возвращает, открыта ли плата для работы
	inline bool Opened()
	{
		return opened;
	}
	// ! Закрыть плату
	void close();

	// ! Здесь можно писать всякие тесты над платой, опрашивать параметры, для дебага
	void Test();
	// ! Внешний старт сбора (преобразования)
	void ExternalStart();
	// ! Внешний стоп сбора (преобразвоания)
	void ExternalStop();


};

// ---------------------------------------------------------------------------
#endif
