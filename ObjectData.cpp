#include "stdafx.h"
#include "ObjectData.h"
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

ObjectData::ObjectData() {
}


ObjectData::~ObjectData() {
}
enum {
	_ENTITY_ = 1,
	_STATS_,
	_CATEGORY_,
	_DESCRIPTION_,
	_RWR_MAIN_,
	_RWR_DATA_,
	_TEXT_,
};
struct Header {
	short type;
	short size;
	//char  Data[];
};
//struct Entity {
//	long GroupID;
//	long SubGroupID;
//	long EntityID;
//
//	// 3d Model ID
//	short ModelID;
//	short ZoomAdjust;
//	short VerticalOffset;
//	short HorizontalOffset;
//	short MissileFlag;
//
//	char Name[32];
//	char  PhotoFile[32];
//};
void ObjectData::LoadData(const std::string &homepath, const std::string &basename, HWND listbox) {
	//string filename = basename + ".HDR";
	//std::ifstream is(filename.c_str(), ios::binary);
	////std::ifstream is("C:\\games\\falcon4\\terrdata\\objects\\KoreaObj.HDR", ios::binary);
	//if (!is) { return; }
	//// Update this each time the object file formats change
	//static const UINT32	FORMAT_VERSION = 0x03087000;
	//UINT32 fileVersion;
	//is.read((char*)&fileVersion, sizeof(fileVersion));
	//if (fileVersion != FORMAT_VERSION) {
	//	assert(fileVersion == FORMAT_VERSION);
	//}
	string tacfilename = homepath + "//tacrefdb.bin";
	std::ifstream is(tacfilename.c_str(), ios::binary);
	if (!is) { return; }
	is.seekg(0, is.end);
	m_Size = (long)is.tellg();
	is.seekg(0, is.beg);
	m_Data = new char[m_Size];
	is.read(m_Data, m_Size);
	//is.close();

//	Header *hdr;
//
//	if (!m_Data)
//		return;
//
//	//*offset = 0;
//	hdr = (Header*)m_Data;
////std::array<Entity, 10> data;
//	Entity data[10];
//	is.seekg(4, is.beg);
//	is.read((char *)data, sizeof(Entity) * 10);
	Entity *ent;
	long entval = 0;
	static TCHAR sbuf[33];
	wstring wsbuf(L"Hello");
	ent = GetFirst(&entval);
	int idx = 0;
	while (ent) {
		//if (ent->SubGroupID == SubGroupID)
		//	lbox->AddItem(ent->EntityID, C_TYPE_ITEM, ent->Name);
		memcpy(sbuf, ent->Name, 32);
		sbuf[32] = 0;
		//wstring wsbuf2(ent->Name);
		SendMessageA(listbox, LB_ADDSTRING,idx++,(LPARAM)sbuf);
		ent = GetNext(&entval);
	}
}

Entity * ObjectData::GetFirst(long * offset) {
	Header *hdr;

	if (!m_Data) { return(NULL); }
	*offset = 0;
	hdr = (Header*)m_Data;
	return((Entity*)&hdr[1]);
}

Entity * ObjectData::GetNext(long * offset) {
	Header *hdr;

	if (!m_Data) { return(NULL); }

	hdr = (Header*)(m_Data + *offset);
	*offset += sizeof(Header) + hdr->size;
	hdr = (Header*)(m_Data + *offset);
	if (hdr->type == _ENTITY_ && *offset < m_Size) { return((Entity*)&hdr[1]); }
	//if (hdr->type == _CATEGORY_ && *offset < m_Size) { 
	//	return((Entity*)&hdr[1]); 
	//}
	return(NULL);
}
