//---------------------------------------------------------------------------

#ifndef ManageH
#define ManageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Inverter.h"

#include <ComCtrls.hpp>
#include <IniFiles.hpp>
#include "Solenoid.h"
//---------------------------------------------------------------------------
class TManageForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gbThickness;
	TStatusBar *StatusBarBottom;
	TBitBtn *bRotation;
	TTimer *InputTimer;
	TCheckBox *bPowerSU;
	TPanel *pSolenoid;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *eAmperage;
	TEdit *eVoltage;
	TEdit *eTemperature;
	TEdit *eFriquencyRot;
	TLabel *Label4;
	TEdit *eResist;
	TLabel *Label5;
	void __fastcall InputTimerTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall bRotationClick(TObject *Sender);
	void __fastcall bPowerSUClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:	// User declarations

	bool butt_enabled;					// блокировка кнопок при движении трубы
	TIniFile* ini;
	Solenoid* ThickSolenoid;
	bool ByTemp;

public:		// User declarations

	__fastcall TManageForm(TComponent* Owner,TIniFile* _ini);

};
//---------------------------------------------------------------------------
extern PACKAGE TManageForm *ManageForm;
//---------------------------------------------------------------------------
#endif
