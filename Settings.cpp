// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Settings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "Global.h"
#include "uFunctions.h"

TSettingsForm *SettingsForm;

// ---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner, TIniFile* _ini)
	: TForm(Owner)
{
	ini = _ini;
}

// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormCreate(TObject *Sender)
{
	LoadFormPos(this, ini);
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormShow(TObject *Sender)
{
	names = new TStringList();
	SelRow = 1;
	SelCol = 1;
	// ищем типоразмеры в файле и добавляем их в ComboBox
	UpdateComboBox();
	names->Add("Diameter");
	names->Add("ThicknessBorder1");
	names->Add("ThicknessBorder2");
	names->Add("Min_Good_Length");
	names->Add("ThicknessDeadZoneStart");
	names->Add("ThicknessDeadZoneFinish");
	names->Add("MinThickness");
	names->Add("MaxThickness");
	names->Add("InSpeed");
	names->Add("WorkSpeed");
	names->Add("OutSpeed");
	// создаем строки с заполненными полями:  "имя настройки" - "текущее значение"
	cbTypeSizeSelect(Sender);
	bDeleteTypeSize->Enabled = true;
	cbTypeSize->Enabled = true;
	bCreateTypeSize->Caption = "Создать";
	StatusBarBottom->Panels->Items[0]->Text = "";
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// при закрытии формы текущий типоразмер становится текущим для всех
	Globals_typesize.setTypesize(cbTypeSize->Text, ini);
}
// ---------------------------------------------------------------------------

void TSettingsForm::UpdateComboBox()
{
	// очищаем весь список элементов ComboBox
	cbTypeSize->Items->Clear();
	TStringList *sections = new TStringList();
	ini->ReadSections(sections);
	// добавляем в список элементов те элементы из ini файла, которые характеризуют типоразмер (префикс "Type_")
	for (int i = 0; i < sections->Count; i++)
	{
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);
	}
	cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(Globals_typesize.name);
	delete sections;
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::ValueListKeyPress(TObject *Sender, wchar_t &Key)
{
	// ловим нажатие на клавишу
	switch (Key)
	{
	case VK_F1:
		WinExec("hh Help.chm::Settings.htm", SW_RESTORE);
		break;
	case 27:
		SettingsForm->Close();
		break;
	case '\r':
			// нажатие на Enter только внутри объекта ValueListEditor
		if (Sender == ValueListEditor)
		{
			if (ValueListEditor->Cells[SelCol][SelRow].IsEmpty())
			{
				StatusBarBottom->Panels->Items[0]->Text = "Введите значение!";
				break;
			}
			else
			{
				if (bCreateTypeSize->Caption == "Создать")
				{ // исправляем значения только у существующего типоразмера
					try
					{
						ValueListEditor->Cells[SelCol][SelRow].ToDouble();
					}
					catch (EConvertError &e)
					{
						e.Message = "Fuck";
						StatusBarBottom->Panels->Items[0]->Text =
							"Неправильное значение!";
						break;
					}
					ini->WriteString("Type_" + cbTypeSize->Text,
						names->Strings[SelRow - 1],
						ValueListEditor->Cells[SelCol][SelRow]);
					StatusBarBottom->Panels->Items[0]->Text =
						"Значение изменено.";
					break;
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::ValueListEditorSelectCell(TObject *Sender,
	int ACol, int ARow, bool &CanSelect)
{
	// сохраняем адрес выделенной ячейки в private переменных.
	SelRow = ARow;
	SelCol = ACol;
	// StatusBarBottom->Panels->Items[1]->Text="Столбец " +IntToStr(SelCol)+", Ряд "+IntToStr(SelRow);
	// this->StatusBarBottom->Refresh();
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::bCreateTypeSizeClick(TObject *Sender)
{
	// пользователь пытается создать типоразмер
	if (bCreateTypeSize->Caption == "Создать")
	{
		ValueListEditor->InsertRow("Название типоразмера", "", false);
		for (int i = 1; i < ValueListEditor->RowCount; i++)
			ValueListEditor->Cells[1][i] = "";
		// даём возможность выбора диаметра только из предложенных значений
		ValueListEditor->ItemProps[1]->PickList->Add("48");
		ValueListEditor->ItemProps[1]->EditStyle = esPickList;
		ValueListEditor->ItemProps[1]->PickList->Add("60");
		ValueListEditor->ItemProps[1]->PickList->Add("73");
		ValueListEditor->ItemProps[1]->PickList->Add("89");
		ValueListEditor->ItemProps[1]->PickList->Add("114");
		cbTypeSize->Enabled = false;
		bDeleteTypeSize->Enabled = false;
		bCreateTypeSize->Caption = "Подтвердить";
	}

	else if (bCreateTypeSize->Caption == "Подтвердить")
	{
		String Sect = "Type_" + ValueListEditor->Cells[1][1];
		bool ok_ts = true;
		// проверяем, все ли норм значения ввел юзер
		try
		{
			for (int i = 0; i < names->Count; i++)
				// double tmp =
					ValueListEditor->Cells[1][i + 2].ToDouble();
		}
		catch (EConvertError &e)
		{
			e.Message = "Fuck";
			StatusBarBottom->Panels->Items[0]->Text = "Неправильное значение!";
			ok_ts = false;
		}

		if (ok_ts)
		{
			for (int i = 0; i < names->Count; i++)
			{
				double tmp = ValueListEditor->Cells[1][i + 2].ToDouble();
				ini->WriteFloat(Sect, names->Strings[i], tmp);
			}
			Globals_typesize.setTypesize(ValueListEditor->Cells[1][1], ini);
			ValueListEditor->DeleteRow(1);
			bCreateTypeSize->Caption = "Создать";
			this->cbTypeSize->Enabled = true;
			this->bDeleteTypeSize->Enabled = true;
			StatusBarBottom->Panels->Items[0]->Text = "Типоразмер создан.";
			UpdateComboBox();
			cbTypeSizeSelect(Sender);
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::bDeleteTypeSizeClick(TObject *Sender)
{
	// пользователь пытается удалить типоразмер
	if (Application->MessageBoxW(L"Вы точно хотите удалить типоразмер?",
		L"Сообщение", MB_YESNO) == 6)
	{
		ini->EraseSection("Type_" + cbTypeSize->Text);
		this->StatusBarBottom->Panels->Items[0]->Text = "Типоразмер удален.";
		if (Globals_typesize.name == cbTypeSize->Text)
		{
			String st0 = cbTypeSize->Items->Strings[0];
			String st1 = cbTypeSize->Items->Strings[1];
			Globals_typesize.setTypesize((cbTypeSize->ItemIndex == 0) ? (st1) :
				(st0), ini);
		}
		UpdateComboBox();
		cbTypeSizeSelect(Sender);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::cbTypeSizeSelect(TObject *Sender)
{
	// изменился типоразмер - нужно показать его настройки. удаляем все поля и заполняем заново
	ValueListEditor->Strings->Clear();
	String sect = "Type_" + cbTypeSize->Text;
	try
	{
		ValueListEditor->InsertRow("Диаметр трубы, мм",
			ini->ReadInteger(sect, "Diameter", 0), true);
		ValueListEditor->ItemProps[0]->ReadOnly = true;

		ValueListEditor->InsertRow("Толщинометрия: порог брака",
			ini->ReadFloat(sect, "ThicknessBorder1", 0.0), true);
		ValueListEditor->InsertRow("Толщинометрия: порог класса 2",
			ini->ReadFloat(sect, "ThicknessBorder2", 0.0), true);
		ValueListEditor->InsertRow("Минимальный годный участок",
			ini->ReadInteger(sect, "Min_Good_Length", 0), true);
		ValueListEditor->InsertRow("Мертвая зона в начале, мм",
			ini->ReadInteger(sect, "ThicknessDeadZoneStart", 0), true);

		ValueListEditor->InsertRow("Мертвая зона в конце, мм",
			ini->ReadInteger(sect, "ThicknessDeadZoneFinish", 0), true);

		ValueListEditor->InsertRow("Минимальная толщина, мм",
			ini->ReadFloat(sect, "MinThickness", 2), true);
		ValueListEditor->InsertRow("Максимальная толщина, мм",
			ini->ReadFloat(sect, "MaxThickness", 9), true);
		ValueListEditor->InsertRow("Скорость вращения на входе, Гц.",
			ini->ReadInteger(sect, "InSpeed", 20), true);
		ValueListEditor->InsertRow("Скорость вращения в работе, Гц.",
			ini->ReadInteger(sect, "WorkSpeed", 35), true);
		ValueListEditor->InsertRow("Скорость вращения на выходе, Гц.",
			ini->ReadInteger(sect, "OutSpeed", 20), true);
		StatusBarBottom->Panels->Items[0]->Text = "";
	}
	catch (...)
	{
		StatusBarBottom->Panels->Items[0]->Text = "Файл настроек поврежден";
		StatusBarBottom->Refresh();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TSettingsForm::FormDestroy(TObject *Sender)
{
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------
