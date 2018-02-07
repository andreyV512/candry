// ---------------------------------------------------------------------------
#ifndef DataAquirerH
#define DataAquirerH
// ---------------------------------------------------------------------------
#include "RawStrobes.h"
#include "CL10M8PCI_SDK2.h"

// ! @brief Класс по сбору данных с платы АЦП Руднев-Шиляев LAn10M8PCI-100.
// ! Постоянно ждет данных с платы, как только получает необходимое число - выдает событие в DataReceiver
// ! и продолжает ждать пакетов
class DataAcquirer : public TThread
{
private:
	// ! Номер платы АЦП в компьютере
	CL10M8PCI_SDK2* card;
	RawStrobes* rawStrobes;

	void __fastcall Execute();
	void pr(AnsiString _msg);

public:

	__fastcall DataAcquirer(CL10M8PCI_SDK2* _card,RawStrobes* _rawStrobes);
};
#endif
