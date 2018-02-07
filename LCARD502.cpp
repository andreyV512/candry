// ---------------------------------------------------------------------------
#pragma hdrstop
#include "LCARD502.h"
#include <Dialogs.hpp>

// ---------------------------------------------------------------------------
LCard_parameters::LCard_parameters()
{
	others.resize(7);
}

LCard502::LCard502()
{
	// Заполним дефолтные настройки
	Parameters.RECV_TOUT = 50;
	Parameters.syncMode = L502_SYNC_INTERNAL; // см. l502api.h
	Parameters.syncStartMode = L502_SYNC_INTERNAL;
	Parameters.frequencyPerChannel = 1000;

	// Номера канала разделения по зонам
	voltToPercent[0] = 10;
	voltToPercent[1] = 20;
	voltToPercent[2] = 50;
	voltToPercent[3] = 100;
	voltToPercent[4] = 200;
	voltToPercent[5] = 500;
	Initialization(getDeviceList());
	if(!setSettings())
		throw(Exception("Ошибка параметров LCard502"));
}

// ---------------------------------------------------------------------------
LCard502::~LCard502()
{
	L502_Free(handle);
}

// ---------------------------------------------------------------------------
void LCard502::Initialization(AnsiString _serial_number)
{
	handle = L502_Create();
	if (handle == NULL)
		throw(Exception("Ошибка создания описателя модуля"));
	int32_t err = L502_Open(handle, _serial_number.c_str());
	if (err)
		throw(Exception(GetErr("Ошибка создания описателя модуля", err)));
}

// ---------------------------------------------------------------------------
AnsiString LCard502::getDeviceList(void)
{
	AnsiString ret;
	int32_t res;

	static char serial[2][L502_SERIAL_SIZE];
	if (L502_GetSerialList(serial, 1, 0, NULL) < 0)
		throw(Exception("Плата LCard-502 не обнаружена"));
	ret = (char*)serial;
	ret.Unique();
	return ret;
}
// ---------------------------------------------------------------------------

bool LCard502::setSettings()
{
	int32_t err = 0;
	// Сколько всего каналов будем собирать
	err = L502_SetLChannelCount(handle, 32);
	if (err)
	{
		ErrMsg("L502_SetLChannelCount", err);
		return (false);
	}
	// Индивидуальные настройки для каждого канала
	for (int i = 0; i < 32; i++)
	{
		err = L502_SetLChannel(handle, i, i, 0, 0, 0);
		if (err)
		{
			ErrMsg("L502_SetLChannel", err);
			return (false);
		}
	}
	// Настраиваем источник частоты синхронизации
	err = L502_SetSyncMode(handle, Parameters.syncMode);
	if (err)
	{
		ErrMsg("L502_SetSyncMode", err);
		return (false);
	}
	// Настраиваем  источник запуска сбора
	err = L502_SetSyncStartMode(handle, Parameters.syncStartMode);
	if (err)
	{
		ErrMsg("L502_SetSyncStartMode", err);
		return (false);
	}
	double f_acq;
	double f_lch;
	// настраиваем частоту сбора с АЦП
	f_acq = Parameters.frequencyCollect;
	f_lch = Parameters.frequencyPerChannel;
	err = L502_SetAdcFreq(handle, &f_acq, &f_lch);
	if (err)
	{
		ErrMsg("L502_SetAdcFreq", err);
		return (false);
	}
	Parameters.frequencyCollect = f_acq;
	Parameters.frequencyPerChannel = f_lch;
	// Записываем настройки в модуль
	err = L502_Configure(handle, 0);
	if (err)
	{
		ErrMsg("L502_Configure", err);
		return (false);
	}
	// Выделим память под данные
	ULONG buffer_size = (ULONG)Parameters.frequencyPerChannel * (ULONG)
		Parameters.channels.size() * 35;
	// Примерное время прохождения 10 метровой трубы через установку
	try
	{
		SourceData.resize(buffer_size);
	}
	catch (...)
	{
		Application->MessageBoxW(L"Не удалось выделить память для буфера L-502",
			L"Ошибка", MB_OK);
		return (false);
	}
	return true;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
vector<double>LCard502::getFrameValue()
{
	std::vector<double>temp, avg;
	temp.resize(32);
	avg.resize(32);

	int32_t err = 0;
	for (int i = 0; i < 5; i++)
	{
		err = L502_AsyncGetAdcFrame(handle, L502_PROC_FLAGS_VOLT, 1000,
			&temp[0]);
		if (!err)
			for (int j = 0; j < (int)temp.size(); j++)
			{
				avg[j] += temp[j];
			}
	}
	for (int i = 0; i < (int)temp.size(); i++)
		avg[i] = avg[i] / 5.0;

	return avg;
}

// ---------------------------------------------------------------------------
AnsiString LCard502::GetErr(AnsiString _msg, int32_t _err)
{
	AnsiString ret = _msg;
	_msg += ": ";
	_msg += L502_GetErrorString(_err);
	return (ret);
}

void LCard502::ErrMsg(AnsiString _msg, int32_t _err)
{
	Application->MessageBoxW(WideString(GetErr(_msg, _err)).c_bstr(),
		L"Ошибка", MB_OK);
}

LCard502* lcard = NULL;
#pragma package(smart_init)
