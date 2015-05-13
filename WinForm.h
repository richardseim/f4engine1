#pragma once
#include <map>
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class WinForm {
public:
	WinForm(HWND hwnd);
	~WinForm();

	void CreateButton(std::string name, std::string text, int x, int y);
	void CreateLabTextBox(std::string name, std::string labeltext, int x, int y, int width);
private:
	HWND m_hwnd;
	int m_nextid;
	std::map<std::string, int> m_name2id;
	std::map<int, HWND> m_id2hwnd;
	void AddToMap(std::string name, HWND hwnd, int id);
};

