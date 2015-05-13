#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>
#include "inputclass.h"
#include "graphicsclass.h"
#include "WinForm.h"



class SystemClass
{
public:
	SystemClass();
	~SystemClass();


	bool Initialize();
	void Shutdown();
	void Run();
	HINSTANCE GetHInst();
	void CreateDXWindow();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void SetupForm();

private:
	bool Frame();
	void InitializeWindowsWithDebugWindow(int&, int&);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	
private:
	std::wstring m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	HWND m_hwnd_dx;

	WinForm* m_form;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;



};
/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK WndProcDX(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
extern SystemClass* g_ApplicationHandle;


