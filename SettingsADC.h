//---------------------------------------------------------------------------

#ifndef SettingsADCH
#define SettingsADCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include <IniFiles.hpp>
#include <ActnCtrls.hpp>
#include <ToolWin.hpp>
#include "cspin.h"
#include <AppEvnts.hpp>

#include "Main.h"
#include "Global.h"
#include "Result.h"
#include "uFRSetADC.h"
//---------------------------------------------------------------------------
class TFormSettADC : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TTabSheet *TabSheet1;
	TGroupBox *gbSensors;
	TApplicationEvents *ApplicationEvents1;
	TTabSheet *TabSheet3;
	TLabel *lParameterA;
	TLabel *lParameterB;
	TLabel *LSizeBuf;
	TLabel *LRate;
	TLabel *lSynchroCaption;
	TComboBox *cbGain;
	TComboBox *cbFrequency;
	TTrackBar *tbSynchroLevel;
	TLabel *lSynchroLevel;
	TLabel *lStartDelay;
	TEdit *eStartDelay;
	TCheckBox *cbSynchroFront;
	TLabel *lMeasuresPacket;
	TComboBox *cbSamplesCount;
	TLabel *lSensorsCount;
	TComboBox *cbSensorsNumber;
	TTabSheet *TabSheet2;
	TButton *bExit;
	TButton *bSave;
	TButton *bRecalculateTube;
	TEdit *eTestTime;
	TLabel *lTestTime;
	TEdit *eStrobesPerPacket;
	TLabel *lStrobesPerPacket;
	TLabel *lChannelAmperage;
	TLabel *lVoltage;
	TCSpinEdit *csAmperage;
	TCSpinEdit *csVoltage;
	TLabel *lThresholdSolenoid;
	TEdit *eResistSolenoid;
	TPageControl *pcMathLogics;
	TTabSheet *tabSheetGAK;
	TLabel *lInaccuracy;
	TLabel *lPeaksCount;
	TLabel *lMaxToMin;
	TCheckBox *cbSmartAnalysis;
	TTrackBar *tbSmartBegin;
	TTrackBar *tbSmartEnd;
	TEdit *eMaxToMin;
	TComboBox *cbPeaksCount;
	TComboBox *cbInaccuracy;
	TTabSheet *tabSheetEKE;
	TLabel *lSensorsMaxToMin;
	TLabel *lMaxEnergy;
	TLabel *lMinEnergy;
	TComboBox *cbACFDivider;
	TLabel *lACFDivider;
	TLabel *lEnergyToMax;
	TLabel *lMaxPeaks;
	TLabel *lDJProcent;
	TLabel *lDJIndex;
	void __fastcall bExitClick(TObject *Sender);
	void __fastcall EditSizeBufKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall cbSensorsNumberChange(TObject *Sender);
	void __fastcall tbSynchroLevelChange(TObject *Sender);
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall bRecalculateTubeClick(TObject *Sender);
	void __fastcall eResistSolenoidKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:	// User declarations

		TEdit	* arA[Globals_max_sensors];		// массив эдитов для параметра А
		TEdit	* arB[Globals_max_sensors];		// массив эдитов для параметра Б
		TLabel	* arl[Globals_max_sensors];		// массив лэйблов
		TFRSetADC* fadc[Globals_max_sensors];

		void SetControlsAble (bool Enable);
		TIniFile *ini;
		Result* result;

public:		// User declarations
	__fastcall TFormSettADC(TComponent* Owner,TIniFile *_ini,Result* _result);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSettADC *FormSettADC;
//---------------------------------------------------------------------------
#endif
