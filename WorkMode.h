// ---------------------------------------------------------------------------

#ifndef WorkModeH
#define WorkModeH
// ---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "Series.hpp"
#include <ComCtrls.hpp>
#include <windows.h>
#include "IniFiles.hpp"
#include <ImgList.hpp>
#include "Solenoid.h"
#include "DataAquirer.h"

// ! @brief Класс, обеспечивающий прохождение всего технологического цикла
// ! @brief Ради одного этого потока и писалась вся программа.
// ! По сути, аггрегирует в себе все функции программы.
class WorkThreadClass : public TThread
{

private:
	// ! Запуск потока
	void __fastcall Execute();

	// ! Текст для надписи в статусбаре в панели 1
	String stext1;
	// ! Текст для надписи в статусбаре в панели 2
	String stext2;

	// ! Тестирует солеиды  false - авария, true - все норм
	bool TestSolenoid();

	// ! Соленоиды в порядке
	bool Solenoid_ok;
	RawStrobes* rawStrobes;

	// ! Обновляет компоненты главной формы, обрабатывает события
	void __fastcall UpdateMainForm();
	void __fastcall ClearCharts();
	void pr(AnsiString _msg);
	__fastcall void AddTickStrobe(unsigned int _tick);
	bool result;
	Solenoid* ThickSolenoid;
	TIniFile *ini;
	void WorkMode(void);
	inline int getvolt(void){return(ThickSolenoid->getchVoltage());};
	ADCBoards* adcboards;
	bool ByTemp;
public:
	__fastcall WorkThreadClass(RawStrobes* _rawStrobes,TIniFile* _ini, ADCBoards* _adcBoards);
	__fastcall ~WorkThreadClass();
};

// ---------------------------------------------------------------------------
#endif
