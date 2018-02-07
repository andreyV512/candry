// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

// using Microsoft::ManagementConsole::Advanced;

#include "Manage.h"
#include "Global.h"
#include "uFunctions.h"
#include "Protocol.h"
#include "LCARD502.h"
#include "A1730.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManageForm *ManageForm;

// ---------------------------------------------------------------------------
__fastcall TManageForm::TManageForm(TComponent* Owner, TIniFile* _ini) : TForm(Owner)
{
	ini = _ini;
	KeyPreview = true;

}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::FormShow(TObject *Sender)
{
	// ������ ����� �� �����
	eFriquencyRot->Text = IntToStr(frConverter->getParameterSpeed(Globals_defaultRotParameter));
	// �������� ������, ������������� ��������� ������ � �������
	InputTimer->Enabled = true;
	butt_enabled = true;
	bPowerSU->Checked = a1730->oSOLPOW->Get();
	StatusBarBottom->Panels->Items[0]->Text = "";
	StatusBarBottom->Refresh();
	ByTemp = ini->ReadBool("Solenoid", "ByTemp", 0);
	Label3->Visible = !ByTemp;
	Label5->Visible = ByTemp;
	eResist->Visible = !ByTemp;
	eTemperature->Visible = ByTemp;
}

// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// ��������� ������, ������������� ��������� ������ � �������
	InputTimer->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------

void __fastcall TManageForm::InputTimerTimer(TObject *Sender)
{
	if (bPowerSU->Checked)
	{
		// ������� ���, ���������� � ����������� ���������
		eAmperage->Text = FloatToStrF(ThickSolenoid->getAmperage(), ffFixed, 4, 3);
		eVoltage->Text = FloatToStrF(ThickSolenoid->getVoltage(), ffFixed, 3, 3);
		if (!ThickSolenoid->OkU())
		{
			a1730->oSOLPOW->Set(false);
			StatusBarBottom->Panels->Items[0]->Text = "���������� ��� ���������";
			StatusBarBottom->Refresh();
			eTemperature->Text = "";
			eResist->Text = "";
			return;
		}
		if (!ByTemp)
		{
			double lresist = ThickSolenoid->getResist();
			if (lresist < 0)
				lresist = -lresist;
			eResist->Text = FloatToStrF(lresist, ffFixed, 3, 1);
			if (!ThickSolenoid->OkResist())
			{
				a1730->oSOLPOW->Set(false);
				StatusBarBottom->Panels->Items[0]->Text = "��������� ������������� �������";
				StatusBarBottom->Refresh();
				return;
			}
		}
		else
		{
			eTemperature->Text = FloatToStrF(ThickSolenoid->getTemp(), ffFixed, 3, 0);
			if (!ThickSolenoid->OkTemp())
			{
				a1730->oSOLPOW->Set(false);
				StatusBarBottom->Panels->Items[0]->Text = "��������� ����������� ��������� (" +
					FloatToStrF(ThickSolenoid->getTemp(), ffFixed, 3, 0) + "C)";
				StatusBarBottom->Refresh();
            }
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormDestroy(TObject *Sender)
{
	delete ThickSolenoid;
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::bRotationClick(TObject *Sender)
{
	if (bRotation->Caption == "�������")
	{
		if (!a1730->oPCHPOW->Get())
		{
			a1730->oPCHPOW->Set(true);
			Sleep(1000);
		}
		if (!frConverter->setParameterSpeed(Globals_defaultRotParameter, StrToInt(eFriquencyRot->Text)))
		{
			TPr::pr("frConverter->setParameterSpeed false");
			StatusBarBottom->Panels->Items[0]->Text = "�� ������� ��������� �������� ��������";
			StatusBarBottom->Refresh();
			return;
		}
		if (!frConverter->startRotation())
		{
			TPr::pr("frConverter->startRotation false");
			StatusBarBottom->Panels->Items[0]->Text = "�� ������� �������� ��������";
			StatusBarBottom->Refresh();
			return;
		}
		DWORD startTick = GetTickCount();
		bool started = false;
		for (; ;)
		{
			if (a1730->iPCHRUN->Get())
			{
				started = true;
				break;
			}
			if (GetTickCount() > startTick + 4000)
				break;
		}
		if (!started)
		{
			TPr::pr("�� ��������� iPCHRUN �� 4 ���n.");
			StatusBarBottom->Panels->Items[0]->Text = "�� ������� ��������� �������� ��������";
			StatusBarBottom->Refresh();
			return;
		}
		bRotation->Caption = "����������";
	}
	else
	{
		if (!frConverter->stopRotation())
		{
			TPr::pr("frConverter->stopRotation() false");
			StatusBarBottom->Panels->Items[0]->Text = "�� ������� ���������� ��";
			StatusBarBottom->Refresh();
			return;
		}
		DWORD startTick = GetTickCount();
		bool stoped = false;
		for (; ;)
		{
			if (!a1730->iPCHRUN->Get())
			{
				stoped = true;
				break;
			}
			if (GetTickCount() > startTick + 2000)
				break;
		}
		if (!stoped)
		{
			TPr::pr("�� ��������� ������ iPCHRUN �� 2 ���.");
			StatusBarBottom->Panels->Items[0]->Text = "�� ������� ���������� ��������";
			StatusBarBottom->Refresh();
			return;
		}
		bRotation->Caption = "�������";
	}
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::bPowerSUClick(TObject *Sender)
{
	a1730->oSOLPOW->Set(((TCheckBox*)Sender)->Checked);
	if (((TCheckBox*)Sender)->Checked)
	{
		eTemperature->Enabled = true;
		eAmperage->Enabled = true;
		eVoltage->Enabled = true;
	}
	else
	{
		StatusBarBottom->Panels->Items[0]->Text = "";
		StatusBarBottom->Refresh();
		eTemperature->Enabled = false;
		eAmperage->Enabled = false;
		eVoltage->Enabled = false;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormCreate(TObject * Sender)
{
	LoadFormPos(this, ini);
	eTemperature->Enabled = false;
	eAmperage->Enabled = false;
	eVoltage->Enabled = false;
	ThickSolenoid = new Solenoid(ini);
}
// ---------------------------------------------------------------------------
