//---------------------------------------------------------------------------
#ifndef CalcMeasH
#define CalcMeasH
//---------------------------------------------------------------------------
#include "uCFFT.h"
#include "Meas.h"
class CalcMeas
{
private:
	void computeAcf(Meas* _meas);
	void computeAcfR(Meas* _meas);
	void computeAcfG(Meas* _meas);
	void computeThickness(Meas* _meas);
	bool RagACF;
	// ! Ссылка на класс, рассчитывающий АКФ по Рагу
	CFFT* FFT;
	// ! Огибание линии АКФ
	__envelope envelope;
	// ! Расчет АКФ в dll от Гака
	__xcorr dll_xcorr;
	// ! Инициализирует сплайн, по которому идет огибание АКФ
	__alloc_spline alloc_spline;
	// ! Освобождение памяти, удаление сплайна
	__free_spline free_spline;

	// ! Определение максимумов огибающей АКФ для одиночного измерения
	void __fastcall findPeaks(Meas* _meas);
	// ! Вектор математических логик - разных подходов к расчету толщины
	std::vector<mathematics*>maths;

	// ! Мат. логика: 0 = GAK, 1 = EKE
	RThickness::mathLogic logic;
	TIniFile* ini;
public:
	CalcMeas(bool _RagACF,RThickness::MathSettings _msettings,TIniFile* _ini);
	~CalcMeas(void);
	void __fastcall Exec(Meas* _meas);
};

#endif
