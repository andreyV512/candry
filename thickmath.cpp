// ---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

// #define GSL_DLL

#include <math.h>
#include <stdio.h>
#include <stdint.h>

// #include <gsl/gsl_errno.h>
// #include <gsl/gsl_spline.h>
// #include <gsl/gsl_interp.h>

#include "thickmath.h"
#include "Global.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// устанавливаем параметры расчетов, берем их из MathSettings
void mathematics::setParameters(RThickness::MathSettings ms)
{
	precision = ms.Precision();
	minThickness = ms.MinThickness();
	maxThickness = ms.MaxThickness();
	minPeaksCount = ms.MinPeaksCount();
	int sensors = ms.SensorCount();
	for (int i = 0; i < sensors; i++)
	{
		calibCoeffs[i] = ms.getCalibrationCoefficients((uint8_t)i);
		minEnergy[i] = ms.MinEnergy(i);
		maxEnergy[i] = ms.MaxEnergy(i);
		maxToMin[i] = ms.MaxToMin(i);
		energyToMax[i] = ms.EnergyToMax(i);
		maxPeaks[i] = ms.MaxPeaks(i);
		djProcent[i] = ms.DJProcent(i);
		djIndex[i] = ms.DJIndex(i);
	}
}

// ---------------------------------------------------------------------------
// Математические расчеты по логике GAK
mathGAK::mathGAK(RThickness::MathSettings ms)
{
	setParameters(ms);
}

// ---------------------------------------------------------------------------
void mathGAK::find_peaks(const double y[], const int32_t Ny, int32_t x_peaks[],
	double y_peaks[], int32_t *Npeaks, int sensorNo)
{
	struct extremum source, prev_max;
	struct extremum extremums[1024];
	struct extremum filt_extremums[1024];

	double *der_1, thresh;
	int32_t i, j;
	int32_t n_extr = 0, n_filt_extr = 0, n_final_extr = 0;

	if (Ny < 2)
	{
		*Npeaks = 0;
		return;
	}

	// thresh = 0.16666667;
	thresh = Globals_MaxToMin;

	der_1 = (double *) malloc(sizeof(double) * (Ny - 1));
	// extremums = (struct extremum*) malloc( sizeof(struct extremum) * (Ny) );
	// filt_extremums = (struct extremum*) malloc( sizeof(struct extremum) * (Ny) );

	// extremums      = new extremum[ Ny / 10 ];
	// filt_extremums = new extremum[ Ny / 10 ];

	// первая производная
	for (i = 0; i < Ny - 1; i++)
	{
		der_1[i] = y[i + 1] - y[i];
	}

	// yy[0] = y[0]; ind[0] = 0;

	for (i = 0; i < Ny - 2; i++)
	{
		if (der_1[i] <= 1.0e-5 && der_1[i + 1] > 1.0e-5)
		{
			extremums[n_extr].x = i + 1;
			extremums[n_extr].y = y[i + 1];
			extremums[n_extr].type = minimum;
			++n_extr;
		}
		if (der_1[i] >= 1.0e-5 && der_1[i + 1] < 1.0e-5)
		{
			extremums[n_extr].x = i + 1;
			extremums[n_extr].y = y[i + 1];
			extremums[n_extr].type = maximum;
			++n_extr;
		}
	}

	*Npeaks = 0;

	for (i = 0; i < n_extr - 2; i++)
	{
		if (extremums[i].type == minimum)
			if (extremums[i + 1].type == maximum)
				if (extremums[i + 2].type == minimum)
					if (extremums[i + 1].y - extremums[i].y >= thresh)
						if (extremums[i + 1].y - extremums[i + 2].y >= thresh)
						{
							filt_extremums[n_filt_extr] = extremums[i + 1];
							n_filt_extr++;
						}
	}

	source.x = 0;
	source.y = 1.0;
	source.type = maximum;
	prev_max = source;

	for (i = 0; i < n_filt_extr; i++)
	{

		if (filt_extremums[i].y < prev_max.y)
		{
			prev_max = filt_extremums[i];

			(void) memcpy(&filt_extremums[n_final_extr++], &filt_extremums[i],
				(n_filt_extr - i) * sizeof(extremum));
			// filt_extremums [ n_final_extr++ ] = filt_extremums[ i ];
			continue;
		}
		else
		{
			for (j = i - 1; j > -1; j--)
			{
				if (filt_extremums[i].y > filt_extremums[j].y)
				{
					(void) memcpy(&filt_extremums[j], &filt_extremums[i],
						(n_filt_extr - i) * sizeof(extremum));

					n_filt_extr--;
					i--;
					// filt_extremums[ j ] = filt_extremums[ i ];
					n_final_extr = j + 1;

					prev_max = filt_extremums[n_final_extr - 1];

					break;
				}
			}
		}

	}

	*Npeaks = n_final_extr;
	for (i = 0; i < n_final_extr; i++)
	{
		x_peaks[i] = filt_extremums[i].x;
		y_peaks[i] = filt_extremums[i].y;
	}

	// free( filt_extremums );
	// free( extremums );

	// delete[] filt_extremums;
	// delete[] extremums;

	free(der_1);
}

// ---------------------------------------------------------------------------
th_status mathGAK::thickness_detect(const int32_t x_peaks[], double y_peaks[],
	const int32_t Npeaks, double* thickness, int sensorNo, double energy, char* _data,
	int _data_size, double* _dj)
{
	// *thickness = UNDETECTED_THICKNESS;
	// Пока полагаем, что неопределённая толщина равна максимальной
	*thickness = maxThickness;
	double a = calibCoeffs[sensorNo].a;
	double b = calibCoeffs[sensorNo].b;

	if (Npeaks < minPeaksCount) // MININUM_PEAKS_COUNT
	{
		return TH_EXTREMUMS_NOT_FOUND; // Экстремумы не обнаружены
	}
	else if (Npeaks == minPeaksCount) // MININUM_PEAKS_COUNT
	{
		// если первый обнаруженный пик отстоит дальше, чем на 6.0 мм
		if (x_peaks[0] > mm2adc(maxThickness, a, b))
		{
			return TH_GREATER_THAN_MAX; // дальше maxThickness
		}
		else if (x_peaks[0] < mm2adc(minThickness, a, b))
		{
			return TH_LESS_THAN_MIN; // ближе minThickness
		}
		else
		{
			return TH_NOT_ENOUGH_INFO; // недостаточно данных для анализа
		}
	}
	else
	{
		if (x_peaks[0] > mm2adc(maxThickness, a, b))
		{
			return TH_GREATER_THAN_MAX; // дальше maxThickness
		}
		else if (x_peaks[0] < mm2adc(minThickness, a, b))
		{
			return TH_LESS_THAN_MIN; // ближе minThickness
		}
		else
		{
			int i, valid = 1;

			double delta = x_peaks[0];
			double *deltas = (double *) malloc(sizeof(double) * Npeaks);

			// for (i = 0; i < Npeaks - 1; i++)
			for (i = 0; i < minPeaksCount; i++)
			{
				// deltas[i] = fabs( x_peaks[i+1] - x_peaks[i] - delta );
				deltas[i] = (x_peaks[i + 1] - x_peaks[i] - delta);

				if (fabs(deltas[i]) > precision)
				{
					free(deltas);
					return TH_BIG_DIFFERENCE;
				}
			}

			double min_delta = 1.0e+9;
			// for( i = 0; i < Npeaks - 1; i++ )
			for (i = 0; i < minPeaksCount; i++)
				if (deltas[i] < min_delta)
					min_delta = deltas[i];

			*thickness = adc2mm(min_delta + delta, a, b);

			free(deltas);

			return TH_OK;
		}
	}
}

// ---------------------------------------------------------------------------
// Математические расчеты по логике EKE
mathEKE::mathEKE(RThickness::MathSettings ms)
{
	setParameters(ms);
}

// ---------------------------------------------------------------------------
void mathEKE::find_peaks(const double y[], const int32_t Ny, int32_t x_peaks[],
	double y_peaks[], int32_t *Npeaks, int sensorNo)
{
	double a = calibCoeffs[sensorNo].a;
	double b = calibCoeffs[sensorNo].b;
	uint32_t start = (uint32_t)mm2adc(minThickness, a, b);
	int end = (uint32_t)mm2adc(maxThickness, a, b);
	if ((end + 2) > Ny) // защита от слишком большой макс толщины
			end = Ny - 2;
	if (start < 2) // защита от слишком маленькой мин толщины
			start = 2;

	int32_t x_max = 0;
	double y_min = y[0];
	for(int i=0;i<start;i++)
		if(y_min > y[i])
			y_min=y[i];

	*Npeaks = 0;
	for (int32_t i = start; i < end; i++)
	{
		// ищем минимум на всем участке (а не предыдущий перед максимумом)
		if (y[i] < y_min)
			y_min = y[i];
		// ищем максимум на всем участке - больше двух соседей и больше всех по ординате
		if (y[i] > y[i - 2] && y[i] > y[i + 2])
		{
			if (y[i] >= y[i - 1] && y[i] >= y[i + 1])
			{
				if (y[i] / y_min > maxToMin[sensorNo])
				{
					x_peaks[*Npeaks] = i;
					y_peaks[*Npeaks] = y[i];
					(*Npeaks)++;
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
th_status mathEKE::thickness_detect(const int32_t x_peaks[], double y_peaks[],
	const int32_t Npeaks, double* thickness, int sensorNo, double energy, char* _data,
	int _data_size, double* _dj)
{
	// Пока полагаем, что неопределённая толщина равна максимальной
	*thickness = maxThickness;

//	if (!computeDJ(_data, _data_size, sensorNo, _dj))
//		return (TH_DJ);
	if (maxPeaks[sensorNo] != 0)
	{
		if (Npeaks > maxPeaks[sensorNo])
			return TH_MAXIMUM_PEAKS;
	}
	if (energy < minEnergy[sensorNo])
		return TH_SMALL_ENERGY;
	if (energy > maxEnergy[sensorNo])
		return TH_BIG_ENERGY;
	if (Npeaks == 0)
		return TH_SMALL_MAXIMUM;
	double ymax = 0;
	int imax = 0;
	for (int i = 0; i < Npeaks; i++)
	{
		if (ymax < y_peaks[i])
		{
			ymax = y_peaks[i];
			imax = i;
		}
	}
	if ((ymax / energy) < energyToMax[sensorNo])
		return TH_MAXIMUM_TO_ENERGY;
	if (!computeDJ(_data, _data_size, sensorNo, _dj))
		return (TH_DJ);
	*thickness = adc2mm(x_peaks[imax], calibCoeffs[sensorNo].a,
		calibCoeffs[sensorNo].b);
	return TH_OK;
}

// ---------------------------------------------------------------------------
bool mathEKE::computeDJ(char* _data, int _size, int _sensor,double* _dj)
{
	if (djProcent[_sensor] == 0 || djIndex[_sensor] == 0)
		return (TH_OK);
	char* p0 = _data;
	char* data = new char[_size];
	char* p1 = data;
	char* p0m = _data + _size;
	int mid = 0;
	for (; p0 < p0m; p0++, p1++)
	{
		*p1 = *p0;
		if (*p1 < 0)
			*p1 = (char)(-(*p1));
		mid += *p1;
	}
	double midD = mid;
	midD /= _size;
	int smax = 0;
	int margin = 0;
	int socket = _size * djProcent[_sensor] / 100;

	char* lpm = data + _size;
	for (int i = 0; i < socket; i++)
	{
		char* lp = data;
		char* lpp = lp;
		for (; lp < lpm; lp++)
		{
			if (*lpp < *lp)
				lpp = lp;
		}
		smax += *lpp;
		*lpp = 0;
	}
	double smaxD = smax;
	smaxD /= socket;
	double dj = smaxD / midD;
	delete data;
	*_dj = dj;
	if (dj <= djIndex[_sensor])
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
// Описание нормированного зондирующего импульса
const double probe_pulse[] =
{0.09346, 0.18692, 0.24299, 0.23364, 0.10280, -0.02804, -0.09346, -0.10280, -0.14019,
	-0.15888, -0.21495, -0.11215, 0.03738, 0.22430, 0.32710, 0.18692, 0.00935,
	-0.08411, -0.10280, 0.00000, 0.08411, 0.13084, 0.15888, 0.09346, -0.13084,
	-0.47664, -0.63551, -0.47664, 0.03738, 0.64486, 1.00000, 0.86916, 0.42056,
	-0.08411, -0.42991, -0.49533, -0.44860, -0.39252, -0.32710, -0.17757, 0.03738,
	0.27103, 0.39252, 0.40187, 0.29907, 0.14019, -0.01869, -0.08411, -0.11215,
	-0.14019, -0.12150, -0.03738, 0.05607, 0.09346, 0.02804, -0.05607, -0.03738,
	0.03738, 0.06542, 0.02804};

// Пересчёт толщины из относительной величины в мм
double adc2mm(const double adc, const double a, const double b)
{
	return (adc * a + b);
}

// Пересчёт толщины из мм в относительную величину (отсчёт АЦП)
double mm2adc(const double mm, const double a, const double b)
{
	return ((mm - b) / a);
}

void _medianfilter(const double* signal, double* result, int N)
{
	// Move window through all elements of the signal
	for (int i = 2; i < N - 2; ++i)
	{
		// Pick up window elements
		double window[5];
		for (int j = 0; j < 5; ++j)
			window[j] = signal[i - 2 + j];
		// Order elements (only half of them)
		for (int j = 0; j < 3; ++j)
		{
			// Find position of minimum element
			int min = j;
			for (int k = j + 1; k < 5; ++k)
				if (window[k] < window[min])
					min = k;
			// Put found minimum element in its place
			const double temp = window[j];
			window[j] = window[min];
			window[min] = temp;
		}
		// Get result - the middle element
		result[i - 2] = window[2];
	}
}

// 1D MEDIAN FILTER wrapper
// signal - input signal
// result - output signal
// N      - length of the signal
void medianfilter(double* signal, double* result, int N)
{
	// Check arguments
	if (!signal || N < 1)
		return;
	// Treat special case N = 1
	if (N == 1)
	{
		if (result)
			result[0] = signal[0];
		return;
	}
	// Allocate memory for signal extension
	double* extension = new double[N + 4];
	// Check memory allocation
	if (!extension)
		return;
	// Create signal extension
	memcpy(extension + 2, signal, N * sizeof(double));
	for (int i = 0; i < 2; ++i)
	{
		extension[i] = signal[1 - i];
		extension[N + 2 + i] = signal[N - 1 - i];
	}
	// Call median filter implementation
	_medianfilter(extension, result ? result : signal, N + 4);
	// Free memory
	delete[]extension;
}

// Функция для "подмешивания" зондирующего импульса к исходному сигналу
void join_probe(const sample_t signal[], sample_t mixed[], int32_t Nsignal,
	int32_t offset)
{
	int i;
	double *pulse, *source, *dmixed;
	double signal_max = 1.0e-5;

	pulse = (double *) malloc(Nsignal*sizeof(double));
	source = (double *) malloc(Nsignal*sizeof(double));
	dmixed = (double *) malloc(Nsignal*sizeof(double));

	for (i = 0; i < Nsignal; i++)
	{
		// копируем исходный сигнал в результирующий буфер,
		// выполняя преобразование типов
		source[i] = dmixed[i] = (double) signal[i];

		// попутно находим максимальное значение сигнала в исходном массиве
		if (signal_max < source[i])
			signal_max = source[i];
	}

	// нормализация зондирующего импульса
	for (i = 0; i < PROBE_PULSE_LENGTH; i++)
		pulse[i] = probe_pulse[i] * signal_max;

	// приклеивание зондирующего импульса к реализации сигнала
	if (offset >= 0)
	{
		memcpy((void *)(dmixed + offset), // куда
			(void *)(pulse), // откуда
			PROBE_PULSE_LENGTH * sizeof(double)); // сколько байт
	}
	else
	{
		int32_t virtual_offset = PROBE_PULSE_LENGTH + offset;

		memcpy((void *)(dmixed), // куда
			(void *)(pulse), // откуда
			PROBE_PULSE_LENGTH * sizeof(double)); // сколько байт

		memcpy((void *)(dmixed + PROBE_PULSE_LENGTH), // куда
			(void *)(source + virtual_offset), // откуда
			(Nsignal - PROBE_PULSE_LENGTH) * sizeof(double)); // сколько байт

	}

	for (i = 0; i < Nsignal; i++)
		mixed[i] = (sample_t) dmixed[i];

	free(pulse);
	free(source);
	free(dmixed);
}
