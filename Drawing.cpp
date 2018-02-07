//---------------------------------------------------------------------------


#pragma hdrstop

#include "Drawing.h"
#include "Protocol.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

DrawResults::DrawResults(TIniFile *_ini)
{
	borders.set_length(2);
// задаем цвета, которые хочет юзер
	Brack = (TColor)_ini->ReadInteger("Color","Brack",0);
	Class2 = (TColor)_ini->ReadInteger("Color","SecondClass",0);
	Good = (TColor)_ini->ReadInteger("Color","Valid",0);
	NotMeasured = (TColor)_ini->ReadInteger("Color","NotMeasured",0);
}
//---------------------------------------------------------------------------

TColor DrawResults::GetColor(double measure)
{
// возвращает цвет зоны в зависимости от толщины в этой зоне
	if (measure >= Globals_mathSettings->MaxThickness())
		return NotMeasured;
	else if (measure > borders[borders.get_high()])
		return Good;
	else if (measure <= borders[0])
		return Brack;
	else
		return Class2;
}
//---------------------------------------------------------------------------
bool DrawResults::IsBrak(double measure)
{
	return(measure <= borders[0]);
}
void DrawResults::MakeDecision( std::vector<double> thickness )
{
// принимает решение по всей трубе и вычисляет зоны отреза
	bool flag=false;		// сигнализирует, что текущий участок будет помечен 2-ым классом, если превысит минимальную годную длину
	bool class2=false;		// участок точно будет помечен 2-ым классом
	short temp=0;			// длина текущего участка в зонах
	short tcut1=0; 			// временный первый рез
	cut1=cut2=0;			// резов по умолчанию нет
	decision="Годно";			// решение не принято
	// ищем зону с минимальной значение толщины
	double min = 10e7;
	for (int i = 0; i < (int)thickness.size(); i++)
	{
		if ( thickness[i] < min )
		{
			min = thickness[i];
			min_thickness = thickness[i];
			min_thickness_zone = i+1;
		}
	}
	min_thickness = (double)Math::RoundTo(min_thickness, -2);
}
//---------------------------------------------------------------------------


