// ---------------------------------------------------------------------------

#pragma hdrstop

#include "rudshel.h"
#include "Protocol.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

_LAN10M8PCI::_LAN10M8PCI()
{
	errorCode = 0;

	opened = false;

	int packetSize = 2020;
	int packetCount = 2;
	quantFreq = (float)(25 * 1e6);
	bufferSize = packetSize * packetCount;
	data = new short[bufferSize];
}

_LAN10M8PCI::~_LAN10M8PCI()
{
	if (opened)
		close();
	delete[]data;
}

bool _LAN10M8PCI::checkBoard(const int& baseAddress)
{
	if (baseAddress < 1)
		return false;

	// Создание события для реагирования на прерывание
	hIRQEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Загружаем драйвер
	LAxFactory = (IFactory*)LAxClient.LoadRegistered(CompanyNameDRV,
		ProductNameDRV, VersionNameDRV, DEVICE_NAME_DEFAULT); // ,
	// L"StaticFactory");
	if (LAxFactory == 0)
	{
		errorCode = -10;
		close();
		return false;
	}
	// Получаем интерфейс драйвера
	pADC = (IADCDevice*)LAxFactory->Create((wchar_t *)"IADCDevice", 0);

	if (pADC == 0)
	{
		close();
		return false;
	}
	errorCode = pADC->Setup(baseAddress,0,0,0);
	if (errorCode <= 0)
	{
		close();
		return false;
	}
	Sleep(250);
	errorCode = pADC->Setup(baseAddress, // Адрес платы
		1,
		1,
		hIRQEvent);
	if (errorCode <= 0)
	{
		close();
		return false;
	}

	opened = true;

	return true;
}

bool _LAN10M8PCI::initSettings(const acquireSettings& settings)
{
	// Инициализируем структуру параметров измерения
	ADCParametersMEMORY1 a;

	int bsize = settings.measureSize * settings.sensorCount *
		settings.strobesPerPacket;
	if (bsize != bufferSize)
	{
		bufferSize = bsize;
		delete[]data;
		data = new short[bufferSize];
	}

	a.m_nStartOf = ADCPARAM_START_COMP; // Запуск по компаратору
	a.m_fFreq = (float)(settings.quantFrequency * 1.0e6);
	// Частота дискретизации, Гц
	a.m_nPacketSize = settings.measureSize; // Размер пачки
	a.m_nPacketNumber = settings.sensorCount * settings.strobesPerPacket;
	// Количество пачек
	a.m_nBeforeHistory = 0; // Размер предыстории

	// Настройки для обоих каналов делаем идентичными, несмотря на то,
	// что на самом деле используется только один канал
	a.m_nDelta[0] = 0; // Смещение 0-го канала
	a.m_nGain[0] = settings.ampGain; // К. усиления 0-го канала
	a.m_nDelta[1] = 0; // Смещение 1-го канала
	a.m_nGain[1] = settings.ampGain; // К. усиления 1-го канала

	// Задание управляющих параметров
	a.m_nControl = ADC_CONTROL_SYNC_TTL | // Синхр-ция от внешн. источника
		// ADC_CONTROL_SYNC_CH0 |
		// ADC_CONTROL_DIFS |       	// Только переменный сигнал
		ADC_CONTROL_DIF0 | ADC_CONTROL_DIF1;

	if (settings.syncType == SYNC_FRONT)
		a.m_nControl |= ADC_CONTROL_SYNC_FRONT; // Синхронизация по фронту
	else
		a.m_nControl |= ADC_CONTROL_SYNC_DECLINE; // Синхронизация по срезу

	a.m_nSyncLevel = settings.syncLevel; // Уровень синхронизации
	a.m_nSyncGain = 1; // Коэффициент усиления внешней синхронизации
	a.m_nStartDelay = settings.startDelay; // Коэффициент задержки при старте

	// В этой переменной драйвер вернет частоту дискретизации после метода Init
	float freq;

	// проверим параметры - метод сам заменит неправильные значения на ближайшие верные
	int e1 = 1024;
	int e2 = 1024;
	errorCode = pADC->Init(ADC_INIT_MODE_CHECK, &a, &freq);
	e1 = errorCode;
	if (errorCode <= 0)
	{
		close();
		return false;
	}
	/*
	 ProtocolForm->SendToProtocol("ЧАСТОТА " + FloatToStr(a.m_fFreq)
	 + " РАЗМЕР ПАКЕТА "	+ IntToStr(a.m_nPacketSize)
	 + " КОЛИЧЕСТВО ПАЧЕК " + IntToStr(a.m_nPacketNumber)
	 + " РАЗМЕР БУФЕРА ПЛАТЫ " + IntToStr(bufferSize) );
	 */
	errorCode = pADC->Init(ADC_INIT_MODE_INIT, &a, &freq);
	e2 = errorCode;
	if (errorCode <= 0)
	{
		close();
		return false;
	}

	float range;
	errorCode = pADC->Get(ADC_GET_RANGE_BIPOLAR, &range);
	if (errorCode <= 0)
	{
		close();
		return false;
	}
	voltRange = (double)range;

	errorCode = pADC->Get(ADC_GET_MAXAMP, &intRange);
	if (errorCode <= 0)
	{
		close();
		return false;
	}

	int datasize;
	errorCode = pADC->Get(ADC_GET_DATASIZE, &datasize);
	int databits;
	errorCode = pADC->Get(ADC_GET_DATABITS, &databits);

	// ProtocolForm->SendToProtocol("Запись одного значания в байтах " + IntToStr(datasize)
	// + " Разрядность АЦП преобразования "	+ IntToStr(databits)
	// + " Частота из INIT" + FloatToStr(freq));

	opened = true;
	return true;
}

short* _LAN10M8PCI::yield()
{
	pADC->ClearInterrupt();
	errorCode = pADC->Start();
	if (errorCode <= 0)
	{
		close();
		return (NULL);
	}
	if (WaitForSingleObject(hIRQEvent, RUDSHEL_WAIT_TIMEOUT) == WAIT_OBJECT_0)
	{
		errorCode =
			pADC->GetData(ADC_DATA_MODE_CONVERT2INT16 | ADC_DATA_MODE_DATACH0,
			data, bufferSize, 0);
		if (errorCode <= 0)
		{
			close();
			return (NULL);
		}
		pADC->ClearInterrupt();
	}
	else
	{
//		errorCode =
//			pADC->GetData(ADC_DATA_MODE_CONVERT2INT16 | ADC_DATA_MODE_DATACH0,
//			data, bufferSize, 0);
		// Сброс прерываний
		pADC->ClearInterrupt();
		// Остановка преобразования
		pADC->Stop();
		pr("_LAN10M8PCI::yield: Не дождались события");
		return (NULL);
	}
	return (data);
}

void _LAN10M8PCI::close()
{
	if(!opened)
		return;
	opened = false;
	if (pADC)
	{
		pADC->Stop();
		pADC->Release();
		pADC = NULL;
	}
	LAxClient.Free();
	if ((int)hIRQEvent > 0)
		CloseHandle(hIRQEvent);
}

void _LAN10M8PCI::Test()
{
	int psize;
	int *packet;
	String str = "";
	errorCode = pADC->Get(ADC_GET_PACKETLIST, &packet);
	errorCode = pADC->Get(ADC_GET_PACKETLIST_SIZE, &psize);

	for (int i = 0; i < psize; i++)
		str += IntToStr(*(packet + i)) + " ";

	TPr::pr("PacketList " + str);
	// ------------------
	String str2 = "";
	int size;
	int *sizelist;
	errorCode = pADC->Get(ADC_GET_SIZELIST, &sizelist);
	errorCode = pADC->Get(ADC_GET_SIZELIST_SIZE, &size);

	for (int i = 0; i < size; i++)
		str2 += IntToStr(*(sizelist + i)) + " ";

	TPr::pr("SizeList " + str2);

}

void _LAN10M8PCI::ExternalStart()
{
	pADC->ClearInterrupt();
	errorCode = pADC->Start();

	if (errorCode <= 0)
	{
		close();
		Application->MessageBoxW(L"Плата РудШел плохо стартанула!",
			L"Печаль", MB_OK);
	}
}

void _LAN10M8PCI::ExternalStop()
{
	pADC->ClearInterrupt();
	pADC->Stop();
}
void _LAN10M8PCI::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}

