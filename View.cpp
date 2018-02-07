// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "View.h"
#include "Global.h"
#include "uFunctions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TViewForm *ViewForm;

// ---------------------------------------------------------------------------
__fastcall TViewForm::TViewForm(TComponent* Owner,Result* _result,
	TIniFile* _ini) : TForm(Owner)
{
	result=_result;
	ini=_ini;
}

void TViewForm::ViewFormInit()
{
	total_charts=Globals_mathSettings->SensorCount();

	byte cols=1; // кол-во столбцов, в которых расположены графики
	short vm=0,hm=20; // верт отступ, гориз отступ
	short chh=(Height-vm*7)/total_charts; // высота чарта
	short chw=Width-hm*2; // ширина чарта

	// создаем графики для результатов
	for(int i=0;i<total_charts;i++)
	{
		arc[i]=new TChart(ViewForm);
		arc[i]->Parent=ViewForm;
		arc[i]->SetBounds(hm+(i%cols)*(chw+hm),(i/cols)*(chh+vm),
			chw,chh);
		arc[i]->Name="Chart_"+IntToStr(i);
		arc[i]->Legend->Visible=false;
		arc[i]->View3D=false;
		arc[i]->BottomAxis->Visible=true;
		arc[i]->BottomAxis->Automatic=false;
		arc[i]->BottomAxis->Minimum=0;
		arc[i]->BottomAxis->Maximum=Globals_max_zones;
		arc[i]->LeftAxis->Automatic=false;
		arc[i]->ParentColor=false;
		arc[i]->Walls->Back->Transparent=false;
		arc[i]->Walls->Back->Transparency=30;
		arc[i]->BackWall->Color=clDkGray;
		arc[i]->Name=L"SensorChart"+IntToStr(i);
		arc[i]->Zoom->Allow=false;
		arc[i]->AllowPanning=TPanningMode::pmNone;
		arc[i]->Align=alTop;
		arc[i]->AddSeries(new TBarSeries(arc[i]));
		// создаем подписи к графикам
		arp[i]=new TPanel(ViewForm);
		arp[i]->Parent=ViewForm;
		arp[i]->Caption=""+IntToStr(i+1);
		arp[i]->Font->Color=clNavy;
		arp[i]->Font->Size=12;
		arp[i]->BevelInner=bvRaised;
		arp[i]->BevelOuter=bvLowered;
		arp[i]->SetBounds((i%cols)*(chw+hm),(i/cols)*(chh+vm),
			chw/20,chh/5);
	}

	arc[total_charts-1]->Align=alClient;
	ViewForm->Refresh();
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormShow(TObject *Sender)
{
	ViewFormInit();
	// thickness_buffer.clear();
	// thickness_buffer.resize(Globals_mathSettings->SensorCount());
	for(int j=0;j<result->SensorCount();j++)
	{
		// std::vector<double> thickness = Globals_rtube.evalTubePerSensor( j );

		// ProtocolForm->SendToProtocol("Датчик "+IntToStr(j+1)+"  "+String(thickness.size()));

		arc[j]->Series[0]->ColorEachPoint=true;
		arc[j]->Series[0]->Marks->Visible=false;
		((TBarSeries *) arc[j]->Series[0])->BarWidthPercent=100;
		((TBarSeries *) arc[j]->Series[0])->OffsetPercent=50;
		((TBarSeries *) arc[j]->Series[0])->SideMargins=true;

		for(int i=0;i<Globals_max_zones;i++)
		{
			if(i<result->zone.Count())
			{
				// thickness_buffer[j].push_back(thickness[i]);
				double tme=result->zone[i]->sensor[j]->thickness_median;
				arc[j]->Series[0]->AddXY((double)i,tme,"",
					DrawResults::GetColor(tme));
			}
			else
				arc[j]->Series[0]->AddXY(i,0,"",clWhite);
		}
		arc[j]->Title->Text->Text="Зона "+IntToStr(1)+", толщина: "+
			FloatToStr(Math::RoundTo
			(result->zone[0]->sensor[j]->thickness_median,-2));
		arc[j]->Axes->Left->SetMinMax(0.0,
			Globals_mathSettings->MaxThickness());

		arc[j]->Series[0]->Tag=j;
		arc[j]->Series[0]->OnClick=(TSeriesClick)&ChartClickSeries;
		arc[j]->Series[0]->OnDblClick=(TSeriesClick)&DoubleChartClickSeries;

		arc[j]->Refresh();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::FormClose(TObject *Sender,TCloseAction &Action)
{
	for(int i=0;i<total_charts;i++)
	{
		// arc[i]->Series->Clear();
		arc[i]->Series[0]->Clear();
		arc[i]->ClearChart();
		delete arc[i];
		delete arp[i];
	}
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::DoubleChartClickSeries(TCustomChart *Sender,
	int ValueIndex,TMouseButton Button,TShiftState Shift,int X,int Y)
{
	if(Shift.Contains(ssCtrl)&&Shift.Contains(ssShift))
	{
		ViewTubeDetailsForm=new TViewTubeDetailsForm(this,result,ini);
		ViewTubeDetailsForm->SendTube(ValueIndex+1,Sender->Tag+1);
		ViewTubeDetailsForm->ShowModal();
		delete ViewTubeDetailsForm;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::ChartClickSeries(TCustomChart *Sender,
	int ValueIndex,TMouseButton Button,TShiftState Shift,int X,int Y)
{
	String str="Просмотр результата: зона "+IntToStr(ValueIndex+1)+
		", датчик "+IntToStr(Sender->Tag+1);
	ViewForm->Caption=str;
	arc[Sender->Tag]->Title->Text->Text="Зона "+IntToStr(ValueIndex+1)+
		", толщина: "+FloatToStr
		(Math::RoundTo(result->zone[ValueIndex]->sensor[Sender->Tag]
		->thickness_median,-2));
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormKeyDown(TObject *Sender,WORD &Key,
	TShiftState Shift)
{
	if(Key==27)
		ViewForm->Close();
	else if(Key==73)
	{
		std::map <th_status,uint32_t> stats=result->GetStats();
		uint32_t total=0;
		// вычисляем общее кол-во обсчитанных измерений
		for(std::map <th_status,uint32_t> ::iterator it=stats.begin();it!=stats.end();it++)
			total+=it->second;
		AnsiString caption="Общее кол-во обсчитанных измерений: ";
		caption+=total;
		caption+="\n";
		// вычисляем процентное отношение каждой ошибки
		for(std::map <th_status,uint32_t> ::iterator it=stats.begin();it!=stats.end();it++)
		{
			double percents=100.0*(double)(it->second)/(double) total;
			if(percents>0.001)
			{
				caption+="   ";
				caption+=result->StatusString(it->first);
				caption.cat_printf(": %7.2lf%%\n",percents);
			}
		}
		WideString w=caption;
		Application->MessageBoxW(w.c_bstr(),L"Статистика",
			MB_ICONINFORMATION);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormResize(TObject *Sender)
{
	// при изменение размеров формы растягиваем графики равномерно
	int height=ClientHeight;
	for(int i=0;i<total_charts;i++)
	{
		arc[i]->Height=height/total_charts;
		arp[i]->Top=i*(height/total_charts);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::FormCreate(TObject *Sender)
{
	LoadFormPos(this,ini);
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormDestroy(TObject *Sender)
{
	SaveFormPos(this,ini);
}
// ---------------------------------------------------------------------------
