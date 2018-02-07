#ifndef RThicknessH
#define RThicknessH

namespace RThickness
{
	/** @brief Структура данных, описывающая точку эскремума функции
	 */
	struct CurvePeak
	{
		// ! абсцисса точки максимума на графике
		int X;
		// ! ордината точки максимума на графике
		double Y;
		// ! Конструктор по умолчанию
		CurvePeak() : X(0), Y(0.0)
		{
		}

		// ! Конструктор с инициализацией значений
		CurvePeak(int _X, double _Y) : X(_X), Y(_Y)
		{
		}
	};
};
#endif
