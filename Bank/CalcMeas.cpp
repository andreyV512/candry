// ---------------------------------------------------------------------------
#pragma hdrstop
#include "CalcMeas.h"
#include <Math.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

CalcMeas::CalcMeas(bool _RagACF, RThickness::MathSettings _msettings,TIniFile* _ini)
{
	RagACF = _RagACF;
	FFT = NULL;
	mathematics* p = new mathGAK(_msettings);
	maths.push_back(p);
	p = new mathEKE(_msettings);
	maths.push_back(p);
	logic = _msettings.CurrentLogic();
	ini=_ini;
}
CalcMeas::~CalcMeas(void)
{
	if(FFT!=NULL)
		delete FFT;
	delete maths[0];
	delete maths[1];
}

void __fastcall CalcMeas::Exec(Meas* _meas)
{
	computeAcf(_meas);
	findPeaks(_meas);
	computeThickness(_meas);
}

void CalcMeas::computeAcf(Meas* _meas)
{
	if (RagACF)
		computeAcfR(_meas);
	else
		computeAcfG(_meas);
}

void CalcMeas::computeAcfR(Meas* _meas)
{
	if (FFT == NULL)
		FFT = new CFFT(_meas->GetSize(),ini);
	else
	{
		if (FFT->GetSize() != _meas->GetSize())
		{
			delete FFT;
			FFT = new CFFT(_meas->GetSize(),ini);
		}
	}
	FFT->Calc((char*)_meas->data);
	double* acf = FFT->GetBUF();
	_meas->size_acf = FFT->GetHalfBufSize();
	for (int i = 0; i < FFT->GetHalfBufSize(); i++)
		_meas->acf[i] = acf[i];
	// если логика ГАК - то нужно АКФ привести к единице (никакой энергии)
	if (logic == RThickness::GAK)
	{
		int size = _meas->size_acf;
		double max = (_meas->acf[1] != 0.0) ? (_meas->acf[1]) : (1);
		for (int i = 0; i < size; i++)
			_meas->acf[i] /= max;
	}
}

void CalcMeas::computeAcfG(Meas* _meas)
{
	// std::vector <sample_t> data;
	// const uint32_t measureCount=measure->data.size()/divider;

	// data=measure->data;

	// в данном массиве будет храниться исходная АКФ, по которой
	// в последствии определяется огибающая
	// std::vector <int32_t> sourceAcf(measureCount);

	bool zeros = true;

	// определение огибающей АКФ
	// функции библиотеки GSL для вычисления кубических сплайнов
	// валятся наглухо с ошибкой при передаче им одинаковых значений в
	// качестве расчётных данных. Поэтому сначала нужно убедиться,
	// что у нас массив содержит хотя бы один отличный от остальных элемент
	bool sameArray = true;

	for (uint32_t i = 0; i < 60; i++)
	{
		if (_meas->data[i] != 0)
		{
			zeros = false;
			break;
		}
	}
	int* sourceAcf = new int[_meas->GetSize()];
	if (!zeros)
	{
		// вычисление исходной АКФ
		dll_xcorr((int8_t*)_meas->data, sourceAcf, _meas->GetSize());

		int32_t accum = sourceAcf[0];
		for (int i = 0; i < _meas->GetSize(); i++)
		{
			// исключающее ИЛИ (XOR) одинаковых значений всегда равно нулю
			accum ^= sourceAcf[i];
			// если аккумулятор стал отличным от нуля, значит sourceAcf[i]
			// отличается от предыдущих значений
			if (accum)
			{
				sameArray = false;
				break;
			}
		}
	}
	_meas->size_acf = _meas->GetSize();

	// если массив с исходной АКФ содержит одинаковые значения
	if (sameArray)
	{
		// то и в огибающую АКФ пишем одни нули
		double* p = _meas->acf;
		double* pp = _meas->acf + _meas->GetSize();
		for (; p < pp; p++)
			* p = 0;
	}
	else
	{
		gsl_interp_accel* accel_ptr;
		gsl_spline* spline_ptr;

		alloc_spline(sourceAcf, _meas->GetSize(), &accel_ptr, &spline_ptr);

		// определение огибающей АКФ
		envelope(sourceAcf, _meas->GetSize(), _meas->acf, accel_ptr,
			spline_ptr);

		// разкомментарить для того, чтобы посмотреть АКФ без огибающей
		// for (int i=0; i< measureCount; i++)
		// measure->acf[i] = (double) sourceAcf[i];
		free_spline(accel_ptr, spline_ptr);
	}

	// если логика ГАК - то нужно АКФ привести к единице (никакой энергии)
	if (logic == RThickness::GAK)
	{
		int size = _meas->size_acf;
		double max = (_meas->acf[1] != 0.0) ? (_meas->acf[1]) : (1);
		for (int i = 0; i < size; i++)
			_meas->acf[i] /= max;
	}
}

void __fastcall CalcMeas::findPeaks(Meas* _meas)
{
	int measureCount = _meas->size_acf;
	// так надежнее, в CFFT массив всегда в 2 раза меньше

	// не зная заранее кол-во пиков, предполагаем
	// их кол-во равным measureSettings.measureCount
	std::vector<int32_t>xPeaks(measureCount);
	std::vector<double>yPeaks(measureCount);
	int32_t Npeaks;

	maths[logic]->find_peaks(_meas->acf, measureCount, &xPeaks[0], &yPeaks[0],
		&Npeaks, _meas->n_sensor);

	_meas->acfPeaks.clear();

	for (int32_t i = 0; i < Npeaks; i++)
		_meas->acfPeaks.push_back(RThickness::CurvePeak(xPeaks[i], yPeaks[i]));
}

void CalcMeas::computeThickness(Meas* _meas)
{
	double energy = _meas->acf[1]; // сама энергия
	int Npeaks = _meas->acfPeaks.size();

	std::vector<int32_t>xPeaks(Npeaks);
	std::vector<double>yPeaks(Npeaks);
	for (int i = 0; i < Npeaks; i++)
	{
		xPeaks[i] = _meas->acfPeaks[i].X;
		yPeaks[i] = _meas->acfPeaks[i].Y;
	}
	_meas->thCode = maths[logic]->thickness_detect(&xPeaks[0], &yPeaks[0],
		Npeaks, &_meas->thickness, _meas->n_sensor, energy,_meas->data,_meas->GetSize(),&_meas->dj);
	if(_meas->thickness!=0)
		_meas->thickness=(double)Math::RoundTo(_meas->thickness,-2);
}

