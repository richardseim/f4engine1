
#include "WinForm.h"
#include "SystemClass.h"
using namespace std;

WinForm::WinForm(HWND hwnd) {
	m_hwnd = hwnd;
	m_nextid = 0;
}


WinForm::~WinForm() {
}

void WinForm::CreateButton(string name, string text, int x, int y) {
	int id = m_nextid++;
	HWND wctld = CreateWindowA("BUTTON", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, 100, 32, m_hwnd, NULL, g_ApplicationHandle->GetHInst(), NULL);
	AddToMap(name, wctld, id);
}

void WinForm::CreateLabTextBox(std::string name, std::string labeltext, int x, int y, int width) {
}

void WinForm::AddToMap(std::string name, HWND hwnd, int id) {
	m_name2id[name] = id;
	m_id2hwnd[id] = hwnd;
}
