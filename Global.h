// ---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include <IniFiles.hpp>
#include "Typesize.h"
#include "CL10M8PCI_SDK2.h"
#include "RThickness.h"
#include "tubefile.h"
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ! Настройки плат Руднев-Шиляев
extern AcquireSettings Globals_adcSettings;
// ! Экземпляр класса Типоразмер
extern Typesize Globals_typesize;
extern double Globals_MaxToMin;
// ! Максимальное количество датчиков
const int Globals_max_sensors=6;
// ! Максимальное количество зон
extern int Globals_max_zones;
// ! параметр частотника в котором хранится скорость врщения
extern int Globals_defaultRotParameter;

using namespace RThickness;
extern MathSettings* Globals_mathSettings;

// ---------------------------------------------------------------------------
#endif
