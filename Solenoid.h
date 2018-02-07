// ---------------------------------------------------------------------------
#ifndef SolenoidH
#define SolenoidH
// ---------------------------------------------------------------------------
// ! ����� ������ � �����������
#include <IniFiles.hpp>

class Solenoid
{
public:
	// ! ����������� �� ���������
	Solenoid(TIniFile* _ini);
	~Solenoid(void);
	// ! @brief ����������� � �����������
	// ! �������� �������� ���� ���������
	double getAmperage();
	// ! �������� �������� ���������� ���������
	double getVoltage();
	// ! ���������� �������� ����������� ���������
	double getResist();
	// ! ���������, �� ���������� �� ��������
	bool OkResist();
	bool OkU(void);
	AnsiString GetUIR(void);
	inline int getchVoltage(void)
	{
		return (chVoltage);
	}
	double getTemp();
	bool OkTemp();

private:
	// ! ����������� �������������
	double Devider;
	// ! ����� L-Card, �� ������� ��������� ����� ����
	int chAmperage;
	int bbb[40];
	// ! ����� L-Card, �� ������� ��������� ����� ����������
	int chVoltage;
	// ! ����������� ������� �����������, ���� �������� ������� ��������
	double AlarmLevel;
	double AlarmULevel;
	double DifULevel;
		// ! ���������� �������� ����������� ���������

	// ! ��������� ���������� �����������
	float TempStart;
	// ! ��������� ���������� �������������
	float ResistStart;
		// ! ������������� ����������� �������������
	float TRC;
	// ! ����������� ����������� � �������� �������, ���� ������� ������� ��������
	float AlarmTemp;

};

#endif
