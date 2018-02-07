//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include "RawStrobes.h"
#include "Calcers.h"

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
#include <IniFiles.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include <DBCtrls.hpp>
#include <ToolWin.hpp>
#include "Password.h"
#include "SettingsADC.h"
#include "View.h"
#include "Settings.h"
#include "Manage.h"
#include "WorkMode.h"
#include "tubefile.h"
#include "Drawing.h"
#include "SMS.h"
#include "Inverter.h"
#include <Classes.hpp>
#include "TestMode.h"

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *bTest;
	TButton *bWork;
	TButton *bView;
	TButton *bManage;
	TButton *bCancelWork;
	TMainMenu *MainMenu1;
	TMenuItem *menuJob;
	TMenuItem *menuTube;
	TMenuItem *menuSettings;
	TMenuItem *menuView;
	TMenuItem *menuTypeSize;
	TMenuItem *menuMeasures;
	TMenuItem *menuWork;
	TMenuItem *menuQuit;
	TMenuItem *menuDiagnostics;
	TMenuItem *menuTest;
	TMenuItem *menuSignalsState;
	TMenuItem *menuProtocol;
	TMenuItem *menuSaveTube;
	TMenuItem *menuLoadTube;
	TMenuItem *menuManagement;
	TMenuItem *menuTestAdvantech;
	TMenuItem *menuCrashForward;
	TMenuItem *menuColors;
	TMenuItem *menuTestSMSConnection;
	TGroupBox *gbThickness;
	TChart *ThicknessChart;
	TStatusBar *StatusBarBottom;
	TStatusBar *StatusBarTop;	TCheckBox *cbInterruptView;
	TSaveDialog *SaveToFileDialog;
	TOpenDialog *OpenDialogFromFile;
	TLabeledEdit *eThicknessBorder1;
	TLabeledEdit *eThicknessBorder2;
	TPanel *pResult;
	TPanel *pLength;
	TPanel *pVelocity;
	TFlowPanel *FlowPanel;
	TStaticText *StaticText1;
	TComboBox *cbTypeSize;
	TStaticText *lMinGoodLength;
	TEdit *eMin_Good_Length;
	TMemo *Memo1;
	TBarSeries *Series3;
	TTimer *TimerZone;
	void __fastcall MainFormInit(TObject *Sender);			// выравнивает объекты на форме, работает в начале работы и при Resize
// стандартные функции
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
// нажатие пунктов главного меню
	void __fastcall menuTypeSizeClick(TObject *Sender);
	void __fastcall menuMeasuresClick(TObject *Sender);
	void __fastcall menuQuitClick(TObject *Sender);
	void __fastcall menuSignalsStateClick(TObject *Sender);
	void __fastcall menuSaveTubeClick(TObject *Sender);
	void __fastcall menuLoadTubeClick(TObject *Sender);
	void __fastcall menuTestAdvantechClick(TObject *Sender);
	void __fastcall menuF1Click(TObject *Sender);
	void __fastcall menuProtocolClick(TObject *Sender);
	void __fastcall menuTestSMSConnectionClick(TObject *Sender);
	void __fastcall menuColorsClick(TObject *Sender);
// нажатие кнопок на форме
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall bViewClick(TObject *Sender);
	void __fastcall bWorkClick(TObject *Sender);
	void __fastcall bManageClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall bCancelWorkClick(TObject *Sender);
	void __fastcall gbThicknessClick(TObject *Sender);
	void __fastcall bGoodTubeClick(TObject *Sender);
	void __fastcall bClass2TubeClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TimerZoneTimer(TObject *Sender);

private:	// User declarations
	RawStrobes* rawStrobes;
	Calcers* calcers;
	int calcers_count;
	void pr(AnsiString _msg);
	std::vector<double>thickness;

	// настройки типоразмеров
	void LoadSettings(void);		// читает настройки из ini файла, заполняет едиты, устанавли пороги, мертвые зоны
	TIniFile *ini;							// ини файл с настройками
	void UpdateComboBox();					// обновляет ComboBox с типоразмерами (после добавления или удаления, в начале работы)

	// рабочий режим
	WorkThreadClass *workthread;            // тред работы
	TestMode* testMode;

	// блокировка
	void SetControlsAble(bool state, TButton *exc1 = NULL, TMenuItem *exc2 = NULL,
			 TButton *exc3 = NULL, TButton *exc4 = NULL );	// блокировать или разблокировать кнопки
	void PutSummaryResultOnChart(std::vector<double> thickness );	// рисует данные на графике
	bool test_timer;
	int TestZoneSize;
	bool bCancel;
//	_LAN10M8PCI *adcBoards;
	ADCBoards* adcboards;

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	void __fastcall ClearCharts(void);			// очищает все графики
	bool work_result;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

