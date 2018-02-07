// ---------------------------------------------------------------------------
#ifndef SolenoidH
#define SolenoidH
// ---------------------------------------------------------------------------
// ! Класс работы с соленоидами
#include <IniFiles.hpp>

class Solenoid
{
public:
	// ! Конструктор по умолчанию
	Solenoid(TIniFile* _ini);
	~Solenoid(void);
	// ! @brief Конструктор с параметрами
	// ! Получить значение тока соленоида
	double getAmperage();
	// ! Получить значение напряжения соленоида
	double getVoltage();
	// ! Рассчитать значение температуры соленоида
	double getResist();
	// ! Проверяет, не перегрелся ли соленоид
	bool OkResist();
	bool OkU(void);
	AnsiString GetUIR(void);
	inline int getchVoltage(void)
	{
		return (chVoltage);
	}
	double getTemp();
	bool OkTemp();

private:
	// ! Коэффициент сопротивления
	double Devider;
	// ! Канал L-Card, на который подключен вывод тока
	int chAmperage;
	int bbb[40];
	// ! Канал L-Card, на который подключен вывод напряжения
	int chVoltage;
	// ! Критический уровень температуры, выше которого считаем перегрев
	double AlarmLevel;
	double AlarmULevel;
	double DifULevel;
		// ! Рассчитать значение температуры соленоида

	// ! Расчетная нормальная температура
	float TempStart;
	// ! Расчетная нормальное сопротивление
	float ResistStart;
		// ! Температурный коэффициент сопротивления
	float TRC;
	// ! Критическая температура в градусах цельсия, выше которой считаем перегрев
	float AlarmTemp;

};

#endif
