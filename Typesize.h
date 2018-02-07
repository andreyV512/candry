//---------------------------------------------------------------------------
#ifndef TypesizeH
#define TypesizeH
#include <System.hpp>
#include <vector.h>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------
class Typesize
{
public:
	// ! Имя типоразмера
	String name;
	// ! Диаметр трубы(60,73,89)
	int diameter;
	// ! Массив порогов - [0] - порог брака, [1] - порог класса 2 и т.д.
	DynamicArray<double>borders;
	// ! Минимальный годный участок
	int min_good_length;
	// ! Минимальная определяемая толщина (для математики)
	double min_detected;
	// ! Максимально определяемая толщина (для математики)
	double max_detected;
	// ! Скорость вращения модуля в работе
	vector<int>workSpeed;
	// ! Скорость вращения модуля при входе трубы
	vector<int>inSpeed;
	// ! Скорость вращения модуля при выходе трубы
	vector<int>outSpeed;
	// ! Задержка перед выставлением рабочей скорости
	int inDelay;
	// ! Номинальная толщина
	double nominal_thickness;
	int ThicknessDeadZoneStart;
	int	ThicknessDeadZoneFinish;
	// ! Устанавливает типоразмер (читает из ini и генерит)
	void setTypesize(String TypesizeName,TIniFile *ini);
};
#endif
