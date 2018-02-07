//---------------------------------------------------------------------------
#pragma hdrstop
#include "Typesize.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
void Typesize::setTypesize(String TypesizeName,TIniFile *ini)
{
	String Sector = "Type_"+TypesizeName;
	name = TypesizeName;
	diameter = ini->ReadInteger(Sector,"Diameter", 2);
	borders.set_length(2);
	borders[0] = ini->ReadFloat(Sector, "ThicknessBorder1", 0.0);
	borders[1] = ini->ReadFloat(Sector, "ThicknessBorder2", 0.0);
	if (borders[1] == 0.0)
		borders.set_length(1);

	min_good_length = ini->ReadInteger(Sector,"Min_Good_Length", 2);
	min_detected = (float)ini->ReadFloat(Sector,"MinThickness", 2.0);
	max_detected = (float)ini->ReadFloat(Sector,"MaxThickness", 2.0);
	nominal_thickness = (float)ini->ReadFloat(Sector,"NominalThickness", 5.0);
	inDelay = ini->ReadInteger(Sector , "InDelay" , 2000);
	ThicknessDeadZoneStart=(int)ini->ReadFloat(Sector,"ThicknessDeadZoneStart",0);
	ThicknessDeadZoneFinish=(int)ini->ReadFloat(Sector,"ThicknessDeadZoneFinish",0);
}

