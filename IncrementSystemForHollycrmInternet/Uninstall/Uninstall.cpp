// Uninstall.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Uninstall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			if (_taccess_s(_T("IncrementSystem.exe"),0) == 0)
			{
				SHELLEXECUTEINFO sei = {0};

				sei.cbSize = sizeof (SHELLEXECUTEINFO);
				sei.lpVerb = _T("Open");
				sei.lpFile = _T("msiexec.exe");
				sei.lpParameters = _T("/x {2A09FBE9-682A-45C9-9395-8049BCD26062}");
				sei.nShow = SW_HIDE;
				sei.hInstApp = AfxGetInstanceHandle();

				ShellExecuteEx (&sei);
			}
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
