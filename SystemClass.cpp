#include "SystemClass.h"

#include "Resource.h"

/////////////
// GLOBALS //
/////////////
//namespace {
	SystemClass* g_ApplicationHandle = 0;
//}

	const int IDEV_LOAD = 1000;
	const int IDEV_DDL_TAC = 1001;

LRESULT CALLBACK CBWndProc(HWND hwnd,  UINT msg, WPARAM wParam, LPARAM lParam) {
	if (g_ApplicationHandle) {
		return g_ApplicationHandle->WndProc(hwnd, msg, wParam, lParam);
	} else {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
LRESULT CALLBACK CBWndProcDX(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (g_ApplicationHandle) {
		return g_ApplicationHandle->WndProcDX(hwnd, msg, wParam, lParam);
	} else {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
//const bool FULL_SCREEN = false;

SystemClass::SystemClass() {
	m_Input = 0;
	m_Graphics = 0;
	m_form = 0;
	m_hinstance = 0;
	m_hwnd = 0;
	m_hwnd_dx = 0;
}

SystemClass::~SystemClass() {
}

bool SystemClass::Initialize() {
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindowsWithDebugWindow(screenWidth, screenHeight);
	//InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();




	//// Initialize the graphics object.
	//result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd_dx);
	//if (!result) {
	//	return false;
	//}

	return true;
}

void SystemClass::Shutdown() {
	// Release the graphics object.
	if (m_Graphics) {
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void SystemClass::Run() {
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done) {
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			done = true;
		} else {
			// Otherwise do the frame processing.
			result = Frame();
			if (!result) {
				done = true;
			}
		}

	}

	return;

}



HINSTANCE SystemClass::GetHInst() {
	return m_hinstance;
}

bool SystemClass::Frame() {
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	// Do the frame processing for the graphics object.
	if (m_Graphics) {
		result = m_Graphics->Frame();
		if (!result) {
			return false;
		}
	}
	return true;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void SystemClass::SetupForm(HWND hwnd) {
	m_form = new WinForm(hwnd);
	m_form->CreateButton(IDEV_LOAD, "btLoad", "Loadx", 50, 50);
	m_form->CreateDDL(IDEV_DDL_TAC, "ddlTac","Tac", 50, 100);
}

void SystemClass::InitializeWindowsWithDebugWindow(int& screenWidth, int& screenHeight) {
	//WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	g_ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	//m_applicationName = L"F4Engine1";
#define MAX_LOADSTRING 100
	TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
	LoadString(m_hinstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(m_hinstance, IDC_F4ENGINE1, szWindowClass, MAX_LOADSTRING);
	m_applicationName = std::wstring(szWindowClass);

	// Setup the windows class for debug window.
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CBWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_F4ENGINE1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_F4ENGINE1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	//wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	//wc.lpfnWndProc = WndProc;
	//wc.cbClsExtra = 0;
	//wc.cbWndExtra = 0;
	//wc.hInstance = m_hinstance;
	//wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	//wc.hIconSm = wc.hIcon;
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//wc.lpszMenuName = NULL;
	//wc.lpszClassName = m_applicationName;
	//wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	//RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN) {
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	} else {
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	//m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(), m_applicationName.c_str(),
	//	WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
	//	posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	m_hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		      CW_USEDEFAULT, 0, 400, 300, NULL, NULL, m_hinstance, NULL);
		
		   if (!m_hwnd)
		   {
			   OutputDebugString(L"Failed to open window");
		      return ;
		   }
		   //hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_F4ENGINE1));
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	//SetupForm();
	// Hide the mouse cursor.
	//ShowCursor(false);

	return;
}
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	g_ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"F4Engine1";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = CBWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN) {
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	} else {
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(), m_applicationName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}
void SystemClass::ShutdownWindows() {
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName.c_str(), m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	g_ApplicationHandle = NULL;

	return;
}
std::string GetLastErrorAsString() {
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return "No error message has been recorded";

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}
void SystemClass::CreateDXWindow() {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	int screenWidth = 400; int screenHeight = 500;

	std::wstring dxclassname = L"DXWindow";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = CBWndProcDX;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = dxclassname.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN) {
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	} else {
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	//m_hwnd_dx = CreateWindowEx(WS_EX_APPWINDOW, dxclassname.c_str(), dxclassname.c_str(),
	//	WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
	//	posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	m_hwnd_dx = CreateWindow(dxclassname.c_str(), dxclassname.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, screenWidth, screenHeight, m_hwnd, NULL, m_hinstance, NULL);
	if (m_hwnd_dx == NULL) {
		OutputDebugStringA(GetLastErrorAsString().c_str());
		OutputDebugString(L"failed to open window dx");
		return;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd_dx, SW_SHOW);
	SetForegroundWindow(m_hwnd_dx);
	SetFocus(m_hwnd_dx);

	// Hide the mouse cursor.
	//ShowCursor(false);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) {
		return;
	}
	// Initialize the graphics object.
	bool result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd_dx);
	if (!result) {
		return ;
	}
	
	return;

}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT SystemClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HWND btLoad;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(m_hinstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_CMDS_NEWDX:
			CreateDXWindow();
			break;

		case IDEV_LOAD:
			btLoadClick();
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		//btLoad = CreateWindowA ("BUTTON", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 50, 100, 32, hWnd, NULL, ApplicationHandle->GetHInst(), NULL);
		SetupForm(hWnd);
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



LRESULT SystemClass::WndProcDX(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
void SystemClass::btLoadClick() {
	string basename = "d:\\\games\\falcon4\\terrdata\\objects\\KoreaObj";
	string homepath = "d:\\\games\\falcon4";
	HWND listbox = m_form->GetWHnd(IDEV_DDL_TAC);
	m_objData = new ObjectData();
	m_objData->LoadData(homepath, basename, listbox);
}