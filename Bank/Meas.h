// ---------------------------------------------------------------------------

#ifndef MeasH
#define MeasH
#include "RThickness.h"
#include <vector>
#include <deque>
#include <map>
#include "thickmath.h"

// ---------------------------------------------------------------------------
class Meas
{
private:
	int size;

public:
	char* data;
	double* acf;
	double thickness;
	double thickness_median;
	int n_sensor;
	int size_acf;
	double dj;
	// ! Код достоверности при определении толщины
	th_status thCode;

	// ! Обнаруженные максимумы АКФ
	std::vector<RThickness::CurvePeak>acfPeaks;

	void Init(int _size, int _n_sensor);
	~Meas(void);

	inline int GetSize(void)
	{
		return (size);
	}

	char* Fill(char* _data);
	void Save(TFileStream* _fs);
	void Load(TFileStream* _fs);
};
#endif
