//---------------------------------------------------------------------------

#ifndef DrawingH
#define DrawingH

#include "Classes.hpp"
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IniFiles.hpp"
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

#include "Global.h"
//---------------------------------------------------------------------------
//! Класс по отрисовке результатов контроля на форме
class DrawResults
{
private:
	//! Цвет барака
	static TColor Brack;
	//! Цвет второго класса
	static TColor Class2;
	//! Цвет годной, хорошей трубы
	static TColor Good;
	//! Цвет неопределенного измерения
	static TColor NotMeasured;

public:
	//! Массив порогов - [0] - порог брака, [1] - порог класса 2 и т.д.
	static DynamicArray <double> borders;
	//! Минимальный годный участок
	static int min_good_length;
	//! Решение по трубе (Брак, Годно, Класс 2)
	static String decision;
	//! Резы для годной трубы
	static short cut1,cut2;
	//! Цвет результата (брак -красный и т.д.)
	static TColor ResultColor;
	//! Минимальная толщина по всей трубе
	static double min_thickness;
	//! Зона с минимальной толщиной
	static int min_thickness_zone;

	//! Конструктор
	DrawResults(TIniFile *_ini);
	//! Возвращает цвет измерения (зоны, датчика, смещения - чего угодно)
	static TColor GetColor(double measure);
	//! Принимает решение по всей трубе и вычисляет зоны отреза
	static void MakeDecision(std::vector<double> thickness);
	static bool IsBrak(double measure);


};

	TColor DrawResults::Brack = clRed;
	TColor DrawResults::Class2 = clBlue;
	TColor DrawResults::Good = clGreen;
	TColor DrawResults::NotMeasured = clLtGray;
	DynamicArray <double> DrawResults::borders;
	int DrawResults::min_good_length = 1;
	short DrawResults::cut1 = 0;
	short DrawResults::cut2 = 0;
	String DrawResults::decision = "";
	TColor DrawResults::ResultColor = clWhite;
	double DrawResults::min_thickness = -1;
	int DrawResults::min_thickness_zone = -1;

#endif
