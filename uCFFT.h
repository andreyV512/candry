#ifndef uCFFTH
#define uCFFTH
#include <IniFiles.hpp>

//! Класс по расчету АКФ сигнала
class CFFT
{

private:
	int *h;
	double *c;
	int n;
	int m;
	int nv;
	void Direct(double* x);
	void Inverse(double* x);
	void Spectrum(double* x);
	int size;
	double* BUF;
	double* BUF_spectro;
	bool CFFT_256;
	int acfBorderLeft;
	int acfBorderRight;
	TIniFile* ini;
public:
	//! Конструктор с размером сигнала
	CFFT(int _size,TIniFile* _ini);
	//! Деструктор
	~CFFT(void);
	//! @brief Расчет АКФ
	//! @param _buf Массив с исходным сигналом, АКФ которого нужно вычислить.
	//! retval Рассчитанный массив возвращается в этой же переменной _buf
	void Calc(double* _buf);
	//! @brief Расчет АКФ
	//! @param _buf Массив с исходным сигналом, АКФ которого нужно вычислить.
	//! retval Рассчитанный массив возвращается в этой же переменной _buf
	void Calc(char* _buf);
	inline int GetBufSize(void)
	{
		return (n);
	};
	inline int GetSize(void)
	{
		return (size);
	};
	inline int GetHalfBufSize(void)
	{
		return (nv);
	};
	inline double* GetBUF(void)
	{
		return(BUF);
	};
	inline double* GetBUFSpectro(void)
	{
		return(BUF_spectro);
	};
	inline int GetBorderLeft(void)
	{
		return(acfBorderLeft);
	}
	inline int GetBorderRight(void)
	{
		return(acfBorderRight);
	}
	void SetBorders(int _acfBorderLeft,int _acfBorderRight);
};
#endif
