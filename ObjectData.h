#pragma once
#include "stdafx.h"
#include <string>
struct Entity {
	long GroupID;
	long SubGroupID;
	long EntityID;

	// 3d Model ID
	short ModelID;
	short ZoomAdjust;
	short VerticalOffset;
	short HorizontalOffset;
	short MissileFlag;

	char Name[32];
	char  PhotoFile[32];
};

class ObjectData {
public:
	ObjectData();
	~ObjectData();
	void LoadData(const std::string &homepath, const std::string &filename, HWND listbox);
	Entity *GetFirst(long *offset);
	Entity *GetNext(long *offset);
private:
	std::string m_filename;
	long m_Size;
	char *m_Data;
};

