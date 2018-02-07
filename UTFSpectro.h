//---------------------------------------------------------------------------

#ifndef UTFSpectroH
#define UTFSpectroH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include "ArrowCha.hpp"
#include "Series.hpp"

//---------------------------------------------------------------------------
class TFSpectro : public TForm
{
public:		// User declarations
	typedef void (__closure *OnBordersDef)(int _BorderLeft,int BorderRight);
__published:	// IDE-managed Components
	TChart *Ch;
	TLineSeries *SSpectro;
	TArrowSeries *SBorders;
	TChart *ChAcf;
	TLineSeries *SAcf;
	TStaticText *STSpectro;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ChClickBackground(TCustomChart *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall SSpectroClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled);
private:	// User declarations
	TIniFile* ini;
	OnBordersDef OnBorders;
	int BorderLeft;
	int BorderRight;
public:		// User declarations
	__fastcall TFSpectro(TIniFile* _ini,OnBordersDef _OnBorders,TComponent* Owner);
	void SetBorders(int _BorderLeft,int _BorderRight);
};
//---------------------------------------------------------------------------
extern PACKAGE TFSpectro *FSpectro;
//---------------------------------------------------------------------------
#endif
