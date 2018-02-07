// ---------------------------------------------------------------------------
#pragma hdrstop
#include "tubefile.h"
#include <stdio.h>
#include <math.h>
#include <iterator>
#include <algorithm>
#include <windows.h>
#include "Series.hpp"
#include "thickmath.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)
#define		DEFAULT_SENSOR_COUNT		(6)
#define		DEFAULT_PRECISION_VALUE		(2)
#define		MAX_PRECISION_VALUE			(10)

using namespace RThickness;

MathSettings::MathSettings()
{
	sensorCount=DEFAULT_SENSOR_COUNT;
	precision=DEFAULT_PRECISION_VALUE;
	minThickness=MIN_THICKNESS;
	maxThickness=MAX_THICKNESS;

	minPeaksCount=1;
	logic=EKE;
	smartMaths=false;
	smartBegin=-10;
	smartEnd=10;

	// инициализируем калибровочные коэффициенты для каждого
	// из измерительных каналов значениями по умолчанию
	// calibCoeffs.resize( SensorCount );
	for(uint8_t i=0;i<sensorCount;i++)
	{
		calibCoeffs[i]=calibrationCoefficients();
		minEnergy[i]=10;
		maxEnergy[i]=1500;
		maxToMin[i]=2;
		energyToMax[i]=0.4;
		maxPeaks[i]=10;
		djProcent[i]=30;
		djIndex[i]=1.8;
	}
}

MathSettings::MathSettings(const uint8_t& SensorCount)
{
	sensorCount=SensorCount;
	precision=DEFAULT_PRECISION_VALUE;
	minThickness=MIN_THICKNESS;
	maxThickness=MAX_THICKNESS;

	minPeaksCount=1;
	logic=EKE;
	smartMaths=false;
	smartBegin=-10;
	smartEnd=10;
	// инициализируем калибровочные коэффициенты для каждого
	// из измерительных каналов значениями по умолчанию
	// calibCoeffs.resize( sensorCount );
	for(uint8_t i=0;i<sensorCount;i++)
	{
		calibCoeffs[i]=calibrationCoefficients();
		minEnergy[i]=10;
		maxEnergy[i]=1500;
		maxToMin[i]=2;
		energyToMax[i]=0.4;
		maxPeaks[i]=10;
		djProcent[i]=30;
		djIndex[i]=1.8;
	}
}

void MathSettings::setSensorCount(const uint8_t& SensorCount)
{
	String strError;

	if(SensorCount>MAX_SENSOR_COUNT)
	{
		strError.sprintf(L"Выбранное кол-во датчиков %d превышает максимально"
			L" возможное %d",SensorCount,MAX_SENSOR_COUNT);
		MessageBox(NULL,strError.w_str(),L"Ошибка",MB_ICONERROR|MB_OK);

		return;
	}

	sensorCount=SensorCount;

	// инициализируем калибровочные коэффициенты для каждого
	// из измерительных каналов значениями по умолчанию
	// calibCoeffs.resize( sensorCount );
	for(uint8_t i=0;i<sensorCount;i++)
		calibCoeffs[i]=calibrationCoefficients();
}

void MathSettings::setPrecision(const uint32_t& Precision)
{
	String strError;

	if(Precision>MAX_PRECISION_VALUE)
	{
		strError.sprintf(L"Выбранная погрешность определения толщины %d"
			L"превышает максимально разрешённую %d",
			Precision,MAX_PRECISION_VALUE);
		MessageBox(NULL,strError.w_str(),L"Ошибка",MB_ICONERROR|MB_OK);

		return;
	}

	precision=Precision;
}

void MathSettings::setCalibrationCoefficients(const uint8_t& sensorNum,
	const calibrationCoefficients& Value)
{
	String strError;

	if(sensorNum>sensorCount)
	{
		strError.sprintf(L"Невозможно изменить калибровочные коэффициенты "
			L"канала %d при общем кол-ве каналов %d",
			sensorNum,sensorCount);
		MessageBox(NULL,strError.w_str(),L"Ошибка",MB_ICONERROR|MB_OK);

		return;
	}

	if(fabs(Value.a)<=1.0e-5)
	{
		strError.sprintf(L"Невозможно изменить калибровочные коэффициенты "
			L"канала %d: Коэффициент а не может быть равен 0.0",
			sensorNum);
		MessageBox(NULL,strError.w_str(),L"Ошибка",MB_ICONERROR|MB_OK);

		return;
	}

	calibCoeffs[sensorNum]=Value;
}
void MathSettings::setMinEnergy(const uint8_t& sensorNum,double value)
{
	minEnergy[sensorNum]=value;
}
void MathSettings::setMaxEnergy(const uint8_t& sensorNum,double value)
{
	maxEnergy[sensorNum]=value;
}
void MathSettings::setMaxToMin(const uint8_t& sensorNum,double value)
{
	maxToMin[sensorNum]=value;
}
void MathSettings::setEnergyToMax(const uint32_t &sensorNum,double value)
{
	energyToMax[sensorNum]=value;
}
void MathSettings::setMaxPeaks(const uint32_t &sensorNum,int value)
{
	maxPeaks[sensorNum]=value;
}
void MathSettings::setDJProcent(const uint32_t &sensorNum,int value)
{
	djProcent[sensorNum]=value;
}
void MathSettings::setDJIndex(const uint32_t &sensorNum,double value)
{
	djIndex[sensorNum]=value;
}

const calibrationCoefficients MathSettings::getCalibrationCoefficients(const uint8_t& sensorNum)
{
	String strError;

	if(sensorNum>sensorCount)
	{
		strError.sprintf(L"Невозможно получить калибровочные коэффициенты "
			L"канала %d при общем кол-ве каналов %d",
			sensorNum,sensorCount);
		MessageBox(NULL,strError.w_str(),L"Ошибка",MB_ICONERROR|MB_OK);

		// на всякий случай вернём значения по умолчанию
		return calibrationCoefficients();
	}

	return calibCoeffs[sensorNum];
}
/* TODO : RAG Все подобные ссылки каленым железом за ненадобностью вместе с констами */
const double MathSettings::MinEnergy(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть минимальную энергию для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return minEnergy[0];
	}
	return minEnergy[sensorNum];
}
const double MathSettings::MaxEnergy(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть максимальную энергию для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return maxEnergy[0];
	}
	return maxEnergy[sensorNum];
}
const double MathSettings::MaxToMin(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть отношение макс/мин для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return maxToMin[0];
	}
	return maxToMin[sensorNum];
}
const double MathSettings::EnergyToMax(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть отношение энергия/макс для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return energyToMax[0];
	}
	return energyToMax[sensorNum];
}

const int MathSettings::MaxPeaks(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть максимальное количество пиков для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return maxPeaks[0];
	}
	return maxPeaks[sensorNum];
}

const int MathSettings::DJProcent(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть DJ процент для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return djProcent[0];
	}
	return djProcent[sensorNum];
}
const double MathSettings::DJIndex(const uint32_t &sensorNum)
{
	if(sensorNum>(uint32_t)sensorCount)
	{
		MessageBox(NULL,L"Невозможно вернуть DJ индекс для канала, так как он больше общего числа каналов",
			L"Ошибка",MB_ICONERROR|MB_OK);
		return djIndex[0];
	}
	return djIndex[sensorNum];
}

void MathSettings::setLogics(int value)
{
	logic=value==0?mathLogic::GAK:mathLogic::EKE;
}

void MathSettings::setMinMaxThicknesses(const double& MinThickness,
	const double& MaxThickness)
{
	if(MinThickness>=MaxThickness)
		return;

	if((MaxThickness<=0.0)||(MinThickness<=0.0))
		return;

	minThickness=MinThickness;
	maxThickness=MaxThickness;
}
void MathSettings::setMinPeaksCount(const uint8_t& minpeaks)
{
	minPeaksCount=minpeaks;
}
void MathSettings::setSmartMaths(const bool& smrt)
{
	smartMaths=smrt;
}

void MathSettings::setSmartBegin(const int8_t& smrtBegin)
{
	smartBegin=smrtBegin;
}
void MathSettings::setSmartEnd(const int8_t& smrtEnd)
{
	smartEnd=smrtEnd;
}

bool MathSettings:: operator==(const MathSettings& Settings)
{
	int i;

	if(sensorCount!=Settings.sensorCount)
		return false;

	if(precision!=Settings.precision)
		return false;

	if(minThickness!=Settings.minThickness)
		return false;

	if(maxThickness!=Settings.maxThickness)
		return false;

	for(i=0;i<sensorCount;i++)
	{
		if(calibCoeffs[i].a!=Settings.calibCoeffs[i].a)
			return false;
		if(calibCoeffs[i].b!=Settings.calibCoeffs[i].b)
			return false;
	}

	return true;
}

