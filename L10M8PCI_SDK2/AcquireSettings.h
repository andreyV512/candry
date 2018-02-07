//---------------------------------------------------------------------------
#ifndef AcquireSettingsH
#define AcquireSettingsH
#include <System.hpp>
#include <IniFiles.hpp>
class AcquireSettings
{
private:
	AnsiString section;
public:
	// ! Длина одного измерения (например, 484, 996 и т.д.)
	int32_t measureSize;
	// ! Количество датчиков
	int32_t sensorCount;
	// ! Частота дискретизации (* 1.0e6)
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
	bool syncFront;
	AcquireSettings(void);
	AcquireSettings(TIniFile* _ini);
	void LoadSettings(TIniFile* _ini);
	void SaveSettings(TIniFile* _ini);
};
#endif
