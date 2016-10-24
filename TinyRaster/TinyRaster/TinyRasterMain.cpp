/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
// OGLWin32.cpp : Defines the entry point for the application.
//

#include "TinyRasterMain.h"
#include <stdio.h>
#include <io.h>
#include <strsafe.h>
#include <fcntl.h>

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "TestApplication.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

void PrintUsage()
{
	printf("Use F1 - F8 keys to select a test case\n");
	printf("F1: Test1: Lines in solid colour\n");
	printf("F2: Test2: Lines in gradient colour using interpolated rasterisation\n");
	printf("F3: Test3: Unfilled polygons\n");
	printf("F4: Test4: Solid filled simple polygons\n");
	printf("F5: Test5: Solid filled complex polygon\n");
	printf("F6: Test6: Filled polygons with alpha blending\n");
	printf("F7: Test7: Gradient filled polygons using interpolated filling\n");
	printf("F8: Test8: A mix of unfilled and filled circles.\n");
}

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	int exitcode = 0;
	//Allocate a console window
	//so that messages can be redirected to stdout
	if (!AllocConsole())
	{
		ErrorExit(TEXT("AllocConsole"));
	}

	FILE* pf_out;
	freopen_s(&pf_out, "CONOUT$", "w", stdout);

	PrintUsage();

	//Create the application instance
	TestApplication* myapp = TestApplication::CreateApplication(hInstance);

	exitcode = myapp->Run();

	myapp->DestroyApplication();

	fclose(pf_out);

	//Free the console window
	if (!FreeConsole())
	{
		ErrorExit(TEXT("FreeConsole"));
	}

	return exitcode;
}
