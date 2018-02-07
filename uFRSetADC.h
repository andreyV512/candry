// ---------------------------------------------------------------------------

#ifndef uFRSetADCH
#define uFRSetADCH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
// ---------------------------------------------------------------------------
class TFRSetADC : public TFrame
{
__published: // IDE-managed Components
	TEdit *MinE;
	TEdit *MaxE;
	TEdit *MaxMin;
	TEdit *EnMax;
	TEdit *MaxPeaks;
	TEdit *DJProcent;
	TEdit *DJIndex;
private: // User declarations
	int pos;
	TIniFile* ini;
	AnsiString section;
public: // User declarations
	__fastcall TFRSetADC(TComponent* Owner,int _pos,TIniFile* _ini);
	void SetEnable(bool _v);
	void SetVisible(bool _v);
	void LoadSettings(void);
	void SaveSettings(void);
};
// ---------------------------------------------------------------------------
extern PACKAGE TFRSetADC *FRSetADC;
// ---------------------------------------------------------------------------
#endif
