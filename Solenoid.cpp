// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Solenoid.h"
#pragma package(smart_init)
#include "LCARD502.h"

Solenoid::Solenoid(TIniFile* _ini)
{
	chAmperage = _ini->ReadInteger("Solenoid", "amperageChannel",0);
	chVoltage = _ini->ReadInteger("Solenoid", "voltageChannel",1);
	AlarmLevel = _ini->ReadFloat("Solenoid", "ResistSolenoid", 91.0);
	AlarmULevel = _ini->ReadFloat("Solenoid", "VoltageSolenoid", 270);
	DifULevel = _ini->ReadFloat("Solenoid", "DifVoltageSolenoid", 20);
	Devider = _ini->ReadFloat("OtherSettings", "Devider", 74.0);

	TempStart = (float)_ini->ReadFloat("Solenoid", "TempStart", 20.0);
	ResistStart = (float)_ini->ReadFloat("Solenoid", "ResistStart", 48.3);
	TRC = (float)_ini->ReadFloat("Solenoid", "TRCoef", 0.0038);
	AlarmTemp = (float)_ini->ReadFloat("Solenoid", "AlarmTemp", 100.0);
}

Solenoid::~Solenoid(void)
{
}

// ---------------------------------------------------------------------------
double Solenoid::getAmperage()
{
	vector<double>Vec = lcard->getFrameValue();
	double I = Vec[chAmperage];
	return I = (I - 2.5) * 10;
}

double Solenoid::getVoltage()
{
	vector<double>Vec = lcard->getFrameValue();
	double U = Vec[chVoltage];
	U = U * Devider;
	return U;
}

double Solenoid::getResist()
{
	double i = getAmperage();
	double u = getVoltage();
	// проверка на отсутствие напряжения
//	if (u < 250)
//		return (AlarmLevel + 1);

//	else
		return (u / i);
}

bool Solenoid::OkResist()
{
	return (getResist() < AlarmLevel);
}

bool Solenoid::OkU()
{
	return ((getVoltage() < DifULevel + AlarmULevel) && (getVoltage() >
		AlarmULevel - DifULevel));
}
AnsiString Solenoid::GetUIR(void)
{
	double i = getAmperage();
	double u = getVoltage();
	double R = (double)(u / i);
	AnsiString ret;
	ret.printf("U=%.3lf I=%.3lf R=%.3lf", u, i, R);
	return (ret);
}
double Solenoid::getTemp()
{
	double Temp = TempStart + (getResist() / ResistStart - 1) / TRC;
	return Temp;
}

bool Solenoid::OkTemp()
{
	bool p = (AlarmTemp > getTemp());
	return p;
}

