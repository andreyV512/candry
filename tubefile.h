// ---------------------------------------------------------------------------

#ifndef tubefileH
#define tubefileH

#include <vcl.h>
#include <vector>
#include <deque>

// Максимально возможное количество датчиков на эксплуатируемых
// в настоящее время установках толщиномеров
#define		MAX_SENSOR_COUNT			(6)
typedef int8_t sample_t;
/* В настоящее время используется 8-разрядный АЦП */

namespace RThickness
{
	// ! @brief Структура калибровочных коэффициентов (пересчет "отсчеты АЦП - реальная толщина")
	// ! @brief Толщина рассчитывается по формуле Y = a * X + b, где Y - толщина в мм,
	// ! X - число отсчетов АЦП между максимумами АКФ
	struct calibrationCoefficients
	{
		calibrationCoefficients() : a(0.066),b(-0.069)
		{
		}
		calibrationCoefficients(double A,double B) : a(A),b(B)
		{
		}
		// ! Значение множителя
		double a;
		// ! Значение постоянного сдвига
		double b;
	};

	// логика расчетов толщины трубы
	enum mathLogic
	{
		GAK=0,
		EKE=1
	};

	// ! Класс математических настроек для расчета трубы
	class MathSettings
	{
	public:
		// ! Конструктор задает все значения по умолчанию
		MathSettings();
		// ! Конструктор задает все значения, исходя из известного числа датчиков
		MathSettings(const uint8_t& SensorCount);

		void setSensorCount(const uint8_t& SensorCount);
		void setPrecision(const uint32_t& Precision);
		void setCalibrationCoefficients(const uint8_t& sensorNum,
			const calibrationCoefficients& Value);
		void setMinEnergy(const uint8_t& sensorNum,double value);
		void setMaxEnergy(const uint8_t& sensorNum,double value);
		void setMaxToMin(const uint8_t& sensorNum,double value);
		void setEnergyToMax(const uint32_t& sensorNum,double value);
		void setMaxPeaks(const uint32_t& sensorNum,int value);
		void setDJProcent(const uint32_t& sensorNum,int value);
		void setDJIndex(const uint32_t& sensorNum,double value);

		void setMinMaxThicknesses(const double& MinThickness,
			const double& MaxThickness);
		void setMinPeaksCount(const uint8_t& minpeaks);
		void setSmartMaths(const bool& smrt);
		void setSmartBegin(const int8_t& smrtBegin);
		void setSmartEnd(const int8_t& smrtEnd);
		void setLogics(int value);

		bool operator==(const MathSettings& Settings);

		const calibrationCoefficients getCalibrationCoefficients(const uint8_t& sensorNum);
		const double MinEnergy(const uint32_t &sensorNum);
		const double MaxEnergy(const uint32_t &sensorNum);
		const double MaxToMin(const uint32_t &sensorNum);
		const double EnergyToMax(const uint32_t &sensorNum);
		const int MaxPeaks(const uint32_t &sensorNum);
		const int DJProcent(const uint32_t &sensorNum);
		const double DJIndex(const uint32_t &sensorNum);

		const inline uint32_t Precision()
		{
			return precision;
		}
		const inline uint8_t SensorCount()
		{
			return sensorCount;
		}
		const inline double MinThickness()
		{
			return minThickness;
		}
		const inline double MaxThickness()
		{
			return maxThickness;
		}
		const inline int MinPeaksCount()
		{
			return minPeaksCount;
		}
		const inline bool IsSmartMaths()
		{
			return smartMaths;
		}
		const inline int SmartBegin()
		{
			return smartBegin;
		}
		const inline int SmartEnd()
		{
			return smartEnd;
		}
		inline mathLogic CurrentLogic()
		{
			return logic;
		}

	private:
		// ! Количество датчиков в установке толщинометрии
		uint8_t sensorCount;

		// ! @brief Максимально допустимое расстояние между пиками на АКФ,
		// ! при котором сигнал расценивается как достоверный, и
		// ! производится дальнейшее определение толщины
		uint32_t precision;

		// ! @brief Калибровочные коэффициенты для каждого из датчиков
		// ! (заменил вектор на явный массив, т.к. векторы нельзя
		// ! напрямую дампить в файлы)
		calibrationCoefficients calibCoeffs[MAX_SENSOR_COUNT];
		// std::vector< calibrationCoefficients >  calibCoeffs;
		// ! Минимальная энергия - персональная для каждого датчика
		double minEnergy[MAX_SENSOR_COUNT];
		// ! Максимальная энергия - персональная для каждого датчика
		double maxEnergy[MAX_SENSOR_COUNT];
		// ! Отношение максимума к минимуму - персональное для каждого датчика
		double maxToMin[MAX_SENSOR_COUNT];
		// ! Отношение энергии к максимума
		double energyToMax[MAX_SENSOR_COUNT];
		// ! Максимальное количество пиков
		int maxPeaks[MAX_SENSOR_COUNT];
		int djProcent[MAX_SENSOR_COUNT];
		double djIndex[MAX_SENSOR_COUNT];

		// ! Минимальная определяемая толщина в мм
		double minThickness;
		// ! Масимальная определяемая толщина в мм
		double maxThickness;
		// ! Минимальное число пиков для рассчета
		int minPeaksCount;
		// ! Используется ли расширенная математика
		bool smartMaths;
		// ! Начальная точка рассчета умной математики
		int smartBegin;
		// ! Конечная точка рассчета умной математики
		int smartEnd;
		// ! Текущая мат логика
		mathLogic logic;
	};

}; // namespace

#endif
