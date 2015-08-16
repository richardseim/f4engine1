
#include "WinForm.h"
#include "SystemClass.h"
using namespace std;
extern SystemClass* g_ApplicationHandle;

WinForm::WinForm(HWND hwnd) {
	m_hwnd = hwnd;
	m_nextid = 0;
}


WinForm::~WinForm() {
}

void WinForm::CreateButton(int id,string name, string text, int x, int y) {
	if (id == 0) { id = m_nextid++; }
	HWND wctld = CreateWindowA("BUTTON", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, 80, 26, m_hwnd, (HMENU)id, ::g_ApplicationHandle->GetHInst(), NULL);
	AddToMap(name, wctld, id);
}

void WinForm::CreateLabTextBox(std::string name, std::string labeltext, int x, int y, int width) {
}

void WinForm::CreateDDL(int id, std::string name, std::string text, int x, int y) {
	if (id == 0) { id = m_nextid++; }
	HWND wctld = CreateWindowA("listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER, x, y, 160, 100, m_hwnd, (HMENU)id, ::g_ApplicationHandle->GetHInst(), NULL);
	AddToMap(name, wctld, id);
}

HWND WinForm::GetWHnd(int id) {
	return m_id2hwnd[id];
}

void WinForm::AddToMap(std::string name, HWND hwnd, int id) {
	m_name2id[name] = id;
	m_id2hwnd[id] = hwnd;
}
