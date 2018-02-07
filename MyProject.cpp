//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>


//---------------------------------------------------------------------------
USEFORM("Protocol.cpp", Pr);
USEFORM("SettingsADC.cpp", FormSettADC);
USEFORM("Settings.cpp", SettingsForm);
USEFORM("Manage.cpp", ManageForm);
USEFORM("UTFSpectro.cpp", FSpectro);
USEFORM("uFRSetADC.cpp", FRSetADC); /* TFrame: File Type */
USEFORM("ViewTubeDetails.cpp", ViewTubeDetailsForm);
USEFORM("View.cpp", ViewForm);
USEFORM("A1730\uFRSigPanel.cpp", FRSigPanel); /* TFrame: File Type */
USEFORM("A1730\SignalsState.cpp", SignalsStateForm);
USEFORM("Main.cpp", MainForm);
USEFORM("ColorSettings.cpp", FormColor);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{

	try
	{
		// Пытаемся открыть мьютекс.
		HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, L"MyApp1.0");
		if(!hMutex)
		  // Мьютекса не существует. То есть,
		  // это первый экземпляр,
		  // создаем мьютекс.
		  hMutex = CreateMutex(0, 0, L"MyApp1.0");
		else
		{
		  // Мьютекс существует , то есть , запущен
		  // второй экземпляр, говорим юзеру что он олень и закрываем прогу.
		  Application->MessageBoxW(L"Приложение уже запущено!!!",L"Ошибка",MB_OK|MB_ICONERROR);
		   return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->Run();

		// Приложение закрывается ,
		// освобождаем мьютекс.
		ReleaseMutex(hMutex);
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
