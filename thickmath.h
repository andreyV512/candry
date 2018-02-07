// ---------------------------------------------------------------------------

#ifndef thickmathH
#define thickmathH

#include <vector>

#include "tubefile.h"
#include "gsl/gsl_spline.h"

// Минимально необходимое для начала анализа кол-во максимумов на АКФ
#define MININUM_PEAKS_COUNT 	(2)

#define THICKMATH_DLL			( L"libthickmath.dll" )

#define UNDETECTED_THICKNESS	((double)(6.0))

#define MAX_THICKNESS           ((double)(6.0))
#define MIN_THICKNESS           ((double)(2.0))

#define PROBE_PULSE_LENGTH		(60)

// ------------------------------------------------------------------------------
// ! Перечисление всех возможных признаков достоврености измерения
enum th_status
{
	TH_OK=0, // толщина стенки определена
		TH_EXTREMUMS_NOT_FOUND=1, // экстремумы АКФ не обнаружены
		TH_GREATER_THAN_MAX=2, // первый пик дальше maxThickness
		TH_LESS_THAN_MIN=3, // первый пик ближе minThickness
		TH_NOT_ENOUGH_INFO=4, // недостаточно данных для анализа
		TH_BIG_DIFFERENCE=5, // разница между пиками больше погрешности
		TH_SMALL_MAXIMUM=6, // отношение максимума к минимуму не удовлетворяет
		TH_BIG_ENERGY=7, // энергия больше верхнего порога
		TH_SMALL_ENERGY=8, // энергия меньше нижнего порога
		TH_MAXIMUM_TO_ENERGY=9, // отношение максимума к энергии ниже заданного
		TH_MAXIMUM_PEAKS=10, // количество пиков больше максимального
		TH_DJ=11
};

enum type
{
	maximum,minimum
};

// ! Структура, описывающая экстремум функции АКФ
struct extremum
{
	// ! Абсцисса (горизонтальная координата экстремума), выражена в отсчетах АЦП
	int32_t x;
	// ! Ордината (вертикальная координата экстремума), выражена в процентах от уровня сигнала
	double y;
	// ! Тип экстремума - максимум или минимум
	enum type type;
};

// ------------------------------------------------------------------------------
// ! Асбтрактный класс различных математик
class mathematics
{
protected:
	// Параметры расчетов - аналогичны MathSettings
	// ! Точность
	uint32_t precision;
	// ! Минимальная распознаваемая толщина
	double minThickness;
	// ! Минимальное число пиков достоверного сигнала
	int minPeaksCount;

	// ! Коэффициенты А и Б для расчетов
	RThickness::calibrationCoefficients calibCoeffs[MAX_SENSOR_COUNT];
	// ! Массив минимальных энергий по каждому датчику
	double minEnergy[MAX_SENSOR_COUNT];
	// ! Массив максимальных энергий пао каждому датчику
	double maxEnergy[MAX_SENSOR_COUNT];
	// ! Массив отношений максимума к миниму
	double maxToMin[MAX_SENSOR_COUNT];
	// ! Массив отношений максимума к энергии
	double energyToMax[MAX_SENSOR_COUNT];
	// ! Массив максимального количества пиков
	int maxPeaks[MAX_SENSOR_COUNT];
	int djProcent[MAX_SENSOR_COUNT];
	double djIndex[MAX_SENSOR_COUNT];

public:
	// ! Функция поиска максимума (-ов) на АКФ
	// ! @param y[] массив точек АКФ
	// ! @param Ny длина массива точек АКФ
	// !	@param x_peaks[] значения Х найденных пиков (заполняется внутри функции)
	// ! @param y_peaks[] значения Y найденных пиков (заполняется внутри функции)
	// ! @param *NPeaks число найденных пиков (заполняется внутри функции)
	// ! @param  sensorNo номер датчика
	virtual void find_peaks(const double y[],
		const int32_t Ny,
		int32_t x_peaks[],
		double y_peaks[],
		int32_t *Npeaks,
		int sensorNo)=0;

	// ! @brief Функция определения толщины со всеми ограничениями, возвращает статус достоверности
	// ! @param  x_peaks[] массив координат Х пиков
	// ! @param  y_peaks[] массив значений пиков
	// ! @param  NPeaks число пиков
	// ! @param  thickness найденная толщина (заполняется внутри функции)
	// ! @param  sensorNo номер датчика
	// ! @param  energy энергия сигнала
	virtual th_status thickness_detect(const int32_t x_peaks[],
		double y_peaks[],
		const int32_t Npeaks,
		double* thickness,
		int sensorNo,
		double energy,
		char* _data,
		int _data_size,
		double* dj)=0;

	// ! Задает параметры для математики - выдергивает их из MathSettings
	void setParameters(RThickness::MathSettings ms);
	// ! Максимальная распознаваемая толщина
	double maxThickness;
};

// ------------------------------------------------------------------------------
// ! Математика по логике GAK (находим все пики, ищем кратность между ними)
class mathGAK : public mathematics
{
public:
	// ! Конструктор расчета по ГАКу
	mathGAK(RThickness::MathSettings ms);
	// ! Переопределенная функция поиска максимума
	void find_peaks(const double y[], const int32_t Ny,
		int32_t x_peaks[],double y_peaks[],
		int32_t *Npeaks,int sensorNo);
	// ! Переопределенная функция расчета толщины и выдачи статуса достоверности
	virtual th_status thickness_detect(
		const int32_t x_peaks[],
		double y_peaks[],
		const int32_t Npeaks,
		double* thickness,
		int sensorNo,
		double energy,
		char* _data,
		int _data_size,
		double* dj);
};

// ------------------------------------------------------------------------------
// ! Математика по логике EKE (находим 1 пик, накладываем ограничения по энергии и проч.)
class mathEKE : public mathematics
{
private:
	bool computeDJ(char* _data,int _size,int _sensor,double* _dj);
public:
	// ! Конструктор расчета по ЕКЕ
	mathEKE(RThickness::MathSettings ms);
	// ! Переопределенная функция поиска максимума
	void find_peaks(const double y[], const int32_t Ny,
		int32_t x_peaks[],double y_peaks[],
		int32_t *Npeaks,int sensorNo);
	// ! Переопределенная функция расчета толщины и выдачи статуса достоверности
	virtual th_status thickness_detect(const int32_t x_peaks[],double y_peaks[],
		const int32_t Npeaks,double* thickness,
		int sensorNo,double energy,char* _data,
		int _data_size,double* _dj);
};

// ------------------------------------------------------------------------------
// ! Пересчет "отсчеты АЦП - толщина в мм"
double adc2mm(const double adc, const double a, const double b);
// ! Пересчет "толщина в мм - отсчеты АЦП"
double mm2adc(const double mm, const double a, const double b);
// ! Медианная фильтрация
void medianfilter(double* signal,double* result,int N);

void join_probe(const sample_t signal[],sample_t mixed[],
	int32_t Nsignal,int32_t offset);

// функции - ссылки на dll c расчетов АКФ

typedef(*__alloc_spline)(int32_t*y,
	int32_t Ny,
	gsl_interp_accel**g_accel,
	gsl_spline**g_spline);

typedef(*__free_spline)(gsl_interp_accel*accel_ptr,gsl_spline*spline_ptr);

typedef void(*__envelope)(int32_t*y,
	int32_t Ny,
	double * curve,
	gsl_interp_accel*accel_ptr,
	gsl_spline*spline_ptr);

typedef int(*__xcorr)(const int8_t data[],
	int32_t acf[],
	int32_t N);

// int opt_thickness_find( const sample_t data[], const int32_t N,
// double *thickness, int32_t *offset );

#endif
