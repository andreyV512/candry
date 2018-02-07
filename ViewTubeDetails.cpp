// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewTubeDetails.h"
#include "uFunctions.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TViewTubeDetailsForm *ViewTubeDetailsForm;

// ---------------------------------------------------------------------------
__fastcall TViewTubeDetailsForm::TViewTubeDetailsForm(TComponent* Owner,Result* _r,
	TIniFile* _ini) : TForm(Owner)
{
	TeeUseMouseWheel=true;
	r=_r;
	ini=_ini;
	fft=NULL;
	fspectro=NULL;
}

// ---------------------------------------------------------------------------
void TViewTubeDetailsForm::SendTube(int _zone,int _sensor)
{
	curr_zone=_zone-1;
	curr_sensor=_sensor-1;
	coeffs=Globals_mathSettings->getCalibrationCoefficients(curr_sensor);
}

// ---------------------------------------------------------------------------
void __fastcall TViewTubeDetailsForm::FormShow(TObject *Sender)
{
	if(fft!=NULL)
		delete fft;
	fft=new CFFT(484,ini);
	divider=ini->ReadInteger("Mathematics","ACFDivider",1);
	MeasureChart->LeftAxis->Automatic=true;
	MeasureChart->LeftAxis->Maximum=Globals_mathSettings->MaxThickness();
	MeasureChart->LeftAxis->Minimum=0;
	MeasureChart->BottomAxis->Automatic=false;
	MeasureChart->BottomAxis->Minimum=0;
	Sensor* sensor=r->zone[curr_zone]->sensor[curr_sensor];
	MeasureChart->BottomAxis->Maximum=sensor->meas.Count();
	MeasureChart->BottomAxis->Increment=50;

	SourceSignalChart->LeftAxis->Automatic=false;
	SourceSignalChart->LeftAxis->Maximum=130;
	SourceSignalChart->LeftAxis->Minimum=-130;
	SourceSignalChart->BottomAxis->Automatic=true; // false;
	SourceSignalChart->BottomAxis->Increment=50;
	SourceSignalChart->Legend->Visible=false;

	ACFChart->LeftAxis->Automatic=true;
	ACFChart->BottomAxis->Automatic=true; // false;

	ACFChart->BottomAxis->Increment=50;
	ACFChart->Legend->Visible=false;

	// создадим серии и настроим их внутри чартов
	MeasureChart->AddSeries(new TBarSeries(MeasureChart));
	MeasureChart->Series[0]->ColorEachPoint=true;
	MeasureChart->Series[0]->Marks->Visible=false;
	((TBarSeries *) MeasureChart->Series[0])->SideMargins=true;
	((TBarSeries*) MeasureChart->Series[0])->BarPen->Width=0,5;
	((TBarSeries *) MeasureChart->Series[0])->BarWidthPercent=100;
	((TBarSeries *) MeasureChart->Series[0])->OffsetPercent=50;
	MeasureChart->AddSeries(new TLineSeries(MeasureChart));
	MeasureChart->Series[1]->Marks->Visible=false;
	MeasureChart->Series[1]->Pen->Color=clYellow;
	MeasureChart->Series[1]->ColorEachPoint=true;

	SourceSignalChart->AddSeries(new TLineSeries(SourceSignalChart));
	SourceSignalChart->AddSeries(new TLineSeries(SourceSignalChart));
	SourceSignalChart->AddSeries(new TLineSeries(SourceSignalChart));
	SourceSignalChart->Series[1]->Pen->Width=2;
	SourceSignalChart->Series[2]->Pen->Width=2;

	ACFChart->AddSeries(new TLineSeries(ACFChart));
	ACFChart->AddSeries(new TLineSeries(ACFChart));
	ACFChart->AddSeries(new TLineSeries(ACFChart));
	ACFChart->AddSeries(new TLineSeries(ACFChart));
	ACFChart->Series[0]->Pen->Width=2;
	ACFChart->Series[1]->Pen->Width=2;
	ACFChart->Series[2]->Pen->Width=1;
	ACFChart->Series[3]->Pen->Width=1;

	bShowRePeaks->Caption=L"";
	filter=true;
	if(ini->ReadBool("Default","UTFSpectroVisible",false))
	{
		fspectro=new TFSpectro(ini,SetSpectroBorders,this);
		PutDataOnSpectro();
		fspectro->SetBorders(fft->GetBorderLeft(),fft->GetBorderRight());
		fspectro->Show();
	}
	PutDataOnCharts(curr_zone,curr_sensor,0);
}

// ---------------------------------------------------------------------------
void TViewTubeDetailsForm::PutDataOnCharts(int z,int s,int o)
{
	bool draw=true;
	// провереяем, не вышел ли номер зоны за рамки
	if(z<0)
	{
		z=0;
		draw=false;
	}
	else if(z>(int)(r->zone.Count()-1))
	{
		z=r->zone.Count()-1;
		draw=false;
	}

	// проверяем, не вышел ли номер датчика за рамки
	if(s<0)
	{
		s=r->SensorCount()-1;
	}
	else if(s>r->SensorCount()-1)
	{
		s=0;
	}
	int zoneMeasures=r->evalZone(s,z)->Count()-1;

	// проверяем, не вышел ли номер измерения за рамки
	if(curr_zone!=z&&o>zoneMeasures)
	{
		o=zoneMeasures;
	}
	else if(o<0&&z>0)
	{
		z--;
		zoneMeasures=r->evalZone(s,z)->Count()-1;
		o=zoneMeasures;
	}
	else if(o<0&&z==0)
	{
		o=1;
		draw=false;
	}
	else if(o>zoneMeasures&&z<r->zone.Count()-1)
	{
		z++;
		zoneMeasures=r->evalZone(s,z)->Count()-1;
		o=0;
	}
	else if(o>zoneMeasures&&z==r->zone.Count()-1)
	{
		o=zoneMeasures;
		draw=false;
	}

	// поместим все измерения в зоне на чарт
	if(draw)
	{
		this->MeasureChart->BottomAxis->Maximum=zoneMeasures;
		measures=r->evalZone(s,z);
		// , filter);
		MeasureChart->Series[0]->Clear();
		MeasureChart->Series[1]->Clear();
		((TBarSeries*) MeasureChart->Series[0])->BarPen->Visible=
			!(zoneMeasures>700);
		for(int i=0;i<zoneMeasures+1;i++)
		{

			if(i<measures->Count())
			{
				if(filter)
					MeasureChart->Series[0]->AddXY(i,
					(*measures)[i]->thickness_median,L"",
					DrawResults::GetColor((*measures)[i]->thickness_median));
				else
					MeasureChart->Series[0]->AddXY(i,(*measures)[i]->thickness,L"",
					DrawResults::GetColor((*measures)[i]->thickness));
			}
			else
				MeasureChart->Series[0]->AddXY(i,0,L"",clWhite);
		}
		MeasureChart->Series[1]->AddXY(o,0,L"",clYellow);
		MeasureChart->Series[1]->AddXY(o,MeasureChart->LeftAxis->Maximum,L"",
			clYellow);
		MeasureChart->Refresh();

		// поместим первичный сигнал на чарт
		// source = Globals::rtube.sourceSignal(s,z,o);
		SourceSignalChart->Series[0]->Clear();
		for(int i=0;i<(int)(*measures)[o]->GetSize()/divider;i++)
		{
			SourceSignalChart->Series[0]->AddXY(i,(*measures)[o]->data[i],L"",
				clPurple);
		}
		SourceSignalChart->Refresh();

		// поместим АКФ на чарт
		cur_meas=(*r->evalZone(s,z))[o];
		// acf = Globals::rtube.autoCorrelation(s,z,o);
		// ACFSize=acf.size();
		ACFChart->Series[0]->Clear();
		for(int i=1;i<(*measures)[o]->size_acf;i++)
		{
			ACFChart->Series[0]->AddXY(i,(*measures)[o]->acf[i],L"",clRed);
		}
		ACFChart->Series[1]->Clear();
		ACFChart->Series[2]->Clear();
		ACFChart->Series[3]->Clear();
		ACFChart->LeftAxis->AdjustMaxMin();
		ShowACFCursor();
		ShowMinMax();
		ACFChart->Refresh();
		// подписи к графику
		sbMeasure->Panels->Items[0]->Text=L"Датчик: "+IntToStr(s+1);
		sbMeasure->Panels->Items[1]->Text=L"Зона: "+IntToStr(z+1);
		sbMeasure->Panels->Items[2]->Text=L"Измерение: "+IntToStr(o+1);
		double th_buf;
		if(filter)
			th_buf=(*measures)[o]->thickness_median;
		else
			th_buf=(*measures)[o]->thickness;
		if(th_buf<Globals_mathSettings->MaxThickness())
			sbMeasure->Panels->Items[3]->Text=
				L"Толщина: "+FloatToStr(Math::RoundTo(th_buf,-2));
		else
			sbMeasure->Panels->Items[3]->Text=L"Толщина: не измерена";
		sbMeasure->Panels->Items[4]->Text="Кол-во измерений в зоне "+
			String(zoneMeasures+1);
		Caption=L"Деталированный просмотр трубы: датчик "+IntToStr(s+1)+
			L", зона "+IntToStr(z+1)+L", измерение "+IntToStr(o+1);
		ClearStatusBars();
		// рассчитаем коэффициенты калибровки датчиков
		coeffs=Globals_mathSettings->getCalibrationCoefficients((uint8_t)s);
	}
	curr_zone=z;
	curr_sensor=s;
	curr_measure=o;
	if(draw)
	{
		ShowPeaks();
		if(fspectro!=NULL)
			PutDataOnSpectro();
	}
}

// ---------------------------------------------------------------------------

void TViewTubeDetailsForm::ShowPeaks()
{
	// выводит информацию о найденнах пиках (вершиНАХ) графика АКФ
	cp=(*(r->evalZone(curr_sensor,curr_zone)))[curr_measure]->acfPeaks;

	rePeaks->Clear();
	if(cp.size()>0)
	{
		rePeaks->Lines->Append(L"Найденные вершины:");
		String str=L"["+IntToStr(cp[0].X)+L"] = "+FloatToStrF(cp[0].Y,
			ffFixed,4,3)+L" : "+IntToStr(cp[0].X);
		rePeaks->Lines->Append(str);

		for(int i=1;i<(int)cp.size();i++)
		{
			str=L"["+IntToStr(cp[i].X)+L"] = "+FloatToStrF(cp[i].Y,ffFixed,
				4,3)+L" : "+IntToStr(cp[i].X-cp[i-1].X);
			rePeaks->Lines->Append(str);
		}
		rePeaks->Lines->Append(L"#############");
	}
	// else
	// rePeaks->Lines->Append(L"Хороших пиков не найдено");
	rePeaks->Lines->Append
		(r->StatusString(((*r->evalZone(curr_sensor,
		curr_zone))[curr_measure]->thCode)));
	double dj=(*(r->evalZone(curr_sensor,curr_zone)))[curr_measure]->dj;
	// if (dj != -1)
	// {
	AnsiString a="DJ=";
	a+=FloatToStrF(dj,ffFixed,4,3);
	rePeaks->Lines->Append(a);
	// }
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::MeasureChartMouseWheelDown(TObject *Sender,
	TShiftState Shift,TPoint &MousePos,bool &Handled)
{
	curr_measure--;
	PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::MeasureChartMouseWheelUp(TObject *Sender,
	TShiftState Shift,TPoint &MousePos,bool &Handled)
{
	curr_measure++;
	PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::FormKeyDown(TObject *Sender,WORD &Key,
	TShiftState Shift)
{
	if(Key==27)
		Close();
}
// ---------------------------------------------------------------------------

void TViewTubeDetailsForm::ShowACFCursor()
{
	if(curr_acf_cur<1)
	{
		curr_acf_cur=1;
	}
	else if(curr_acf_cur>=cur_meas->size_acf)
	{
		curr_acf_cur=cur_meas->size_acf;
	}
	else
	{
		ACFChart->Series[1]->Clear();
		ACFChart->Series[1]->AddXY(curr_acf_cur,ACFChart->LeftAxis->Minimum,L"",
			clBlue);
		ACFChart->Series[1]->AddXY(curr_acf_cur,ACFChart->LeftAxis->Maximum,L"",
			clBlue);
		sbACF->Panels->Items[0]->Text=L"Сэмпл: "+IntToStr(curr_acf_cur);
		sbACF->Panels->Items[1]->Text=
			L"Амплитуда: "+FloatToStr
			(Math::RoundTo(cur_meas->acf[curr_acf_cur],-3));
		sbACF->Panels->Items[2]->Text=
			L"Толщина: "+FloatToStr(coeffs.a*curr_acf_cur+coeffs.b);
		sbACF->Refresh();

		ACFChart->Refresh();
	}
}

// -----------------------------------------------------------------------------------------
void TViewTubeDetailsForm::ShowSignal1()
{
	if(Curr_Signal1<1)
	{
		Curr_Signal1=1;
	}
	else if(Curr_Signal1>=(int)Globals_adcSettings.measureSize/divider)
	{
		Curr_Signal1=Globals_adcSettings.measureSize/divider;
	}
	else
	{
		SourceSignalChart->Series[1]->Clear();
		SourceSignalChart->Series[1]->AddXY(Curr_Signal1,
			SourceSignalChart->LeftAxis->Minimum,L"",clBlue);
		SourceSignalChart->Series[1]->AddXY(Curr_Signal1,
			SourceSignalChart->LeftAxis->Maximum,L"",clBlue);

		sbSource->Panels->Items[0]->Text=L"Сэмпл №1 "+IntToStr(Curr_Signal1);
		sbSource->Panels->Items[2]->Text=
			L"Амплитуда №1: "+FloatToStr
			(Math::RoundTo(cur_meas->data[Curr_Signal1],-3));
		double th=coeffs.a*(abs(Curr_Signal1-Curr_Signal2))+coeffs.b;
		sbSource->Panels->Items[4]->Text=L"Толщина: "+FloatToStr(th);
		SourceSignalChart->Refresh();
	}
}

// ---------------------------------------------------------------------------
void TViewTubeDetailsForm::ShowSignal2()
{
	if(Curr_Signal2<1)
	{
		Curr_Signal2=1;
	}
	else if(Curr_Signal2>=(int)Globals_adcSettings.measureSize/divider)
	{
		Curr_Signal2=Globals_adcSettings.measureSize/divider;
	}
	else
	{
		SourceSignalChart->Series[2]->Clear();
		SourceSignalChart->Series[2]->AddXY(Curr_Signal2,
			SourceSignalChart->LeftAxis->Minimum,L"",clRed);
		SourceSignalChart->Series[2]->AddXY(Curr_Signal2,
			SourceSignalChart->LeftAxis->Maximum,L"",clRed);

		sbSource->Panels->Items[1]->Text=L"Сэмпл №2 "+IntToStr(Curr_Signal2);
		sbSource->Panels->Items[3]->Text=
			L"Амплитуда №2: "+FloatToStr
			(Math::RoundTo(cur_meas->data[Curr_Signal2],-3));
		double th=coeffs.a*(abs(Curr_Signal1-Curr_Signal2))+coeffs.b;
		sbSource->Panels->Items[4]->Text=L"Толщина: "+FloatToStr(th);
		SourceSignalChart->Refresh();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TViewTubeDetailsForm::ACFChartClickSeries(TCustomChart *Sender,
	TChartSeries *Series,int ValueIndex,TMouseButton Button,TShiftState Shift,
	int X,int Y)
{
	// user ищет толщину на графике аффтокорреляции
	ACFChart->SetFocus();
	curr_acf_cur=ValueIndex;
	ShowACFCursor();
}

// ---------------------------------------------------------------------------
void __fastcall TViewTubeDetailsForm::ACFChartMouseWheelDown(TObject *Sender,
	TShiftState Shift,TPoint &MousePos,bool &Handled)
{
	curr_acf_cur--;
	ShowACFCursor();
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::ACFChartMouseWheelUp(TObject *Sender,
	TShiftState Shift,TPoint &MousePos,bool &Handled)
{
	curr_acf_cur++;
	ShowACFCursor();
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::bShowRePeaksClick(TObject *Sender)
{
	if(rePeaks->Visible)
	{
		rePeaks->Visible=false;
		bShowRePeaks->Caption=L"";
		Refresh();
	}
	else
	{
		ACFChart->Width=ViewTubeDetailsForm->Width/2-120;
		rePeaks->Visible=true;
		rePeaks->Left=ACFChart->Left+ACFChart->Width;
		bShowRePeaks->Caption=L"";
		bShowRePeaks->Left=ACFChart->Left-170; // rePeaks->Left - 55
		bShowRePeaks->BringToFront();
		bShowRePeaks->Repaint();
		bShowRePeaks->SetFocus();
		bShowRePeaks->Refresh();
		Refresh();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::MeasureChartKeyDown(TObject *Sender,WORD &Key,
	TShiftState Shift)
{
	if(Shift.Empty())
	{
		switch(Key)
		{
		case 37:
			{
				curr_measure--;
				PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
				break;
			}
		case 39:
			{
				curr_measure++;
				PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
				break;
			}

		case 38:
			{
				curr_sensor--;
				PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
				break;
			}
		case 40:
			{
				curr_sensor++;
				PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
				break;
			}
		case 27:
			{
				Close();
				break;
			}
		}
	}
	else if(Shift.Contains(ssCtrl))
	{
		switch(Key)
		{
		case 37:
			{
				// curr_zone--;
				PutDataOnCharts(curr_zone-1,curr_sensor,curr_measure);
				break;
			}
		case 39:
			{
				// curr_zone++;
				PutDataOnCharts(curr_zone+1,curr_sensor,curr_measure);
				break;
			}
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::ACFChartKeyDown(TObject *Sender,WORD &Key,
	TShiftState Shift)
{
	switch(Key)
	{
	case 37:
		{
			curr_acf_cur--;
			ShowACFCursor();
			break;
		}
	case 39:
		{
			curr_acf_cur++;
			ShowACFCursor();
			break;
		}
	case 27:
		{
			Close();
			break;
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::ACFChartClickBackground(TCustomChart *Sender,
	TMouseButton Button,TShiftState Shift,int X,int Y)
{
	if(Button==mbLeft)
	{
		double x,y;
		ACFChart->Series[0]->GetCursorValues(x,y);

		ACFChart->SetFocus();
		curr_acf_cur=(int)x;
		ShowACFCursor();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::SourceSignalChartClickBackground
	(TCustomChart *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{
	double x,y;
	if(Shift.Contains(ssCtrl))
	{
		SourceSignalChart->Series[0]->GetCursorValues(x,y);

		SourceSignalChart->SetFocus();
		Curr_Signal2=(int)x;
		ShowSignal2();
	}
	else
	{
		SourceSignalChart->Series[0]->GetCursorValues(x,y);

		SourceSignalChart->SetFocus();
		Curr_Signal1=(int)x;
		ShowSignal1();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::SourceSignalChartKeyDown(TObject *Sender,
	WORD &Key,TShiftState Shift)
{
	if(Shift.Empty())
		switch(Key)
		{
		case 37:
			{
				Curr_Signal1--;
				ShowSignal1();
				break;
			}
		case 39:
			{
				Curr_Signal1++;
				ShowSignal1();
				break;
			}
		case 27:
			{
				Close();
				break;
			}
		}
	else if(Shift.Contains(ssCtrl))
		switch(Key)
		{
		case 37:
			{
				Curr_Signal2--;
				ShowSignal2();
				break;
			}
		case 39:
			{
				Curr_Signal2++;
				ShowSignal2();
				break;
			}
		case 27:
			{
				Close();
				break;
			}
		}
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::SourceSignalChartMouseWheelDown(TObject *Sender,
	TShiftState Shift,TPoint &MousePos,bool &Handled)
{
	if(Shift.Contains(ssCtrl))
	{
		Curr_Signal2--;
		ShowSignal2();
	}
	if(Shift.Empty())
	{
		Curr_Signal1--;
		ShowSignal1();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::SourceSignalChartMouseWheelUp(TObject *Sender,
	TShiftState Shift,TPoint &MousePos,bool &Handled)
{
	if(Shift.Contains(ssCtrl))
	{
		Curr_Signal2++;
		ShowSignal2();
	}
	if(Shift.Empty())
	{
		Curr_Signal1++;
		ShowSignal1();
	}
}

// ---------------------------------------------------------------------------
void TViewTubeDetailsForm::ClearStatusBars()
{
	for(int i=0;i<3;i++)
		this->sbACF->Panels->Items[i]->Text="";
	for(int i=0;i<5;i++)
		this->sbSource->Panels->Items[i]->Text="";
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::cbMedianFilterClick(TObject *Sender)
{
	filter=cbMedianFilter->Checked;
	PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::FormClose(TObject *Sender,TCloseAction &Action)
{
	if(fspectro!=NULL)
	{
		ini->WriteBool("Default","UTFSpectroVisible",fspectro->Visible);
		delete fspectro;
	}
	else
		ini->WriteBool("Default","UTFSpectroVisible",false);
	MeasureChart->Series[0]->Clear();
	MeasureChart->Series[1]->Clear();
	ACFChart->Series[0]->Clear();
	ACFChart->Series[1]->Clear();
	SourceSignalChart->Series[0]->Clear();
	SourceSignalChart->Series[1]->Clear();
	SourceSignalChart->Series[2]->Clear();
	MeasureChart->RemoveAllSeries();
	SourceSignalChart->RemoveAllSeries();
	ACFChart->RemoveAllSeries();
}
// ---------------------------------------------------------------------------

void __fastcall TViewTubeDetailsForm::pmDrawMinMaxThicknessClick(TObject *Sender)
{
	pmDrawMinMaxThickness->Checked=!pmDrawMinMaxThickness->Checked;
	ShowMinMax();
}
// ---------------------------------------------------------------------------

void TViewTubeDetailsForm::ShowMinMax()
{
	ACFChart->Series[2]->Clear();
	ACFChart->Series[3]->Clear();
	if(pmDrawMinMaxThickness->Checked)
	{
		double min=mm2adc(Globals_mathSettings->MinThickness(),coeffs.a,coeffs.b);
		double max=mm2adc(Globals_mathSettings->MaxThickness(),coeffs.a,coeffs.b);
		ACFChart->Series[2]->AddXY(min,ACFChart->LeftAxis->Minimum,L"",clGreen);
		ACFChart->Series[2]->AddXY(min,ACFChart->LeftAxis->Maximum,L"",clGreen);
		ACFChart->Series[3]->AddXY(max,ACFChart->LeftAxis->Minimum,L"",clGreen);
		ACFChart->Series[3]->AddXY(max,ACFChart->LeftAxis->Maximum,L"",clGreen);
	}
	ACFChart->Series[2]->RefreshSeries();
	ACFChart->Series[3]->RefreshSeries();
}

void __fastcall TViewTubeDetailsForm::MeasureChartClick(TObject *Sender)
{
	double x,y;
	MeasureChart->Series[0]->GetCursorValues(x,y);
	// user ищет измерения
	MeasureChart->SetFocus();
	curr_measure=int(x);
	PutDataOnCharts(curr_zone,curr_sensor,curr_measure);
}

// ---------------------------------------------------------------------------
void __fastcall TViewTubeDetailsForm::FormCreate(TObject *Sender)
{
	LoadFormPos(this,ini);
}

// ---------------------------------------------------------------------------
void __fastcall TViewTubeDetailsForm::FormDestroy(TObject *Sender)
{
	SaveFormPos(this,ini);

}
// ---------------------------------------------------------------------------
void TViewTubeDetailsForm::PutDataOnSpectro(void)
{
	fspectro->SSpectro->Clear();
	fspectro->SAcf->Clear();
	Meas* m=r->zone[curr_zone]->sensor[curr_sensor]->meas[curr_measure];
	if(fft->GetSize()!=m->GetSize())
	{
		delete fft;
		fft=new CFFT(m->GetSize(),ini);
	}
	fft->Calc(m->data);

	double* buf_spectro=fft->GetBUFSpectro();
	int n=fft->GetBufSize();
	for(int i=0;i<n;i++)
		fspectro->SSpectro->AddXY(i,buf_spectro[i],L"",clBlue);

	double* buf_acf=fft->GetBUF();
	n=fft->GetHalfBufSize();
	for(int i=0;i<n;i++)
		fspectro->SAcf->AddXY(i,buf_acf[i],L"",clRed);
}
// ---------------------------------------------------------------------------
void TViewTubeDetailsForm::SetSpectroBorders(int _BorderLeft,int _BorderRight)
{
	fft->SetBorders(_BorderLeft,_BorderRight);
	PutDataOnSpectro();
}
void __fastcall TViewTubeDetailsForm::SBSFClick(TObject *Sender)
{
	if(fspectro==NULL)
		fspectro=new TFSpectro(ini,SetSpectroBorders,this);
	if(!fspectro->Visible)
	{
		PutDataOnSpectro();
		fspectro->SetBorders(fft->GetBorderLeft(),fft->GetBorderRight());
		fspectro->Show();
	}
}
// ---------------------------------------------------------------------------
