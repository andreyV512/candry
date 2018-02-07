//---------------------------------------------------------------------------
#pragma hdrstop
#include "Global.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
// ! Настройки плат Руднев-Шиляев
AcquireSettings Globals_adcSettings;
// ! Экземпляр класса Типоразмер
Typesize Globals_typesize;
// ! Наши любимые платы
//_LAN10M8PCI *Globals_adcBoards;
// ! Временно поместим сюда настройку отношения пиков
double Globals_MaxToMin;
// ! Максимальное количество зон
int Globals_max_zones = 70;
// ! параметр частотника в котором хранится скорость врщения
int Globals_defaultRotParameter = 4;
MathSettings* Globals_mathSettings=NULL;


