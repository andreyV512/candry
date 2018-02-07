// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UTFSpectro.h"
#include "uFunctions.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "ArrowCha"
#pragma link "Series"
#pragma resource "*.dfm"
TFSpectro *FSpectro;
// ---------------------------------------------------------------------------
__fastcall TFSpectro::TFSpectro(TIniFile* _ini,OnBordersDef _OnBorders,TComponent* Owner)
	: TForm(Owner)
{
	ini=_ini;
	OnBorders=_OnBorders;
	BorderLeft=0;
	BorderRight=0;
}
// ---------------------------------------------------------------------------
void __fastcall TFSpectro::FormCreate(TObject *Sender)
{
	LoadFormPos(this,ini);

}
// ---------------------------------------------------------------------------
void __fastcall TFSpectro::FormDestroy(TObject *Sender)
{
	SaveFormPos(this,ini);
}
// ---------------------------------------------------------------------------
void __fastcall TFSpectro::ChClickBackground(TCustomChart *Sender,TMouseButton Button,
	TShiftState Shift,int X,int Y)
{
	double x,y;
	SSpectro->GetCursorValues(x,y);
	if(Button==mbLeft)
		SetBorders((int)x,BorderRight);
	else if(Button==mbRight)
		SetBorders(BorderLeft,x);
	else
		return;
	if(OnBorders==NULL)
		return;
	OnBorders(BorderLeft,BorderRight);
}
// ---------------------------------------------------------------------------

void __fastcall TFSpectro::SSpectroClick(TChartSeries *Sender,int ValueIndex,TMouseButton Button,
	TShiftState Shift,int X,int Y)
{
	double x,y;
	SSpectro->GetCursorValues(x,y);
	if(Button==mbLeft)
		SetBorders((int)x,BorderRight);
	else if(Button==mbRight)
		SetBorders(BorderLeft,x);
	else
		return;
	if(OnBorders==NULL)
		return;
	OnBorders(BorderLeft,BorderRight);
}
// ---------------------------------------------------------------------------
void __fastcall TFSpectro::FormResize(TObject *Sender)
{
	Ch->Left=0;
	Ch->Width=ClientWidth;
	Ch->Top=0;
	Ch->Height=ClientHeight/2-STSpectro->Height;
	STSpectro->Left=0;
	STSpectro->Width=ClientWidth;
	STSpectro->Top=Ch->Height;
	ChAcf->Left=0;
	ChAcf->Width=ClientWidth;
	ChAcf->Top=STSpectro->Top+STSpectro->Height;
	ChAcf->Height=ClientHeight/2-STSpectro->Height;
}
// ---------------------------------------------------------------------------

void TFSpectro::SetBorders(int _BorderLeft,int _BorderRight)
{
	BorderLeft=_BorderLeft;
	BorderRight=_BorderRight;
	if(BorderLeft<0)
		BorderLeft=0;
	int maxX=SSpectro->MaxXValue();
	if(BorderLeft>maxX)
		BorderLeft=maxX;
	if(BorderRight<0)
		BorderRight=0;
	if(BorderRight>maxX)
		BorderRight=maxX;
	if(BorderRight<BorderLeft)
		BorderRight=BorderLeft;
	int maxY=SSpectro->MaxXValue();
	SBorders->Clear();
	SBorders->AddArrow(BorderLeft,0,BorderLeft,maxY,"",clRed);
	SBorders->AddArrow(BorderRight,0,BorderRight,maxY,"",clWebDarkRed);
	AnsiString a="Левая граница: ";
	a+=BorderLeft;
	a+=", Правая граница: ";
	a+=BorderRight;
	STSpectro->Caption=a;

}
// ---------------------------------------------------------------------------

void __fastcall TFSpectro::FormMouseWheel(TObject *Sender,TShiftState Shift,int WheelDelta,
	TPoint &MousePos,bool &Handled)
{
	TPoint p=Ch->ScreenToClient(MousePos);
	if(	p.x<0||p.x>=Ch->Width||p.y<0||p.y>Ch->Height)
		return;
	if(!Shift.Contains(ssMiddle))
	{
		if(WheelDelta<0)
			BorderLeft-=1;
		else
			BorderLeft+=1;
	}
	else
	{
		if(WheelDelta<0)
			BorderRight-=1;
		else
			BorderRight+=1;
	}
	SetBorders(BorderLeft,BorderRight);
	if(OnBorders==NULL)
		return;
	OnBorders(BorderLeft,BorderRight);
}
// ---------------------------------------------------------------------------
