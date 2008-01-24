
#include "StdAfx.h"
#include "../include/hoe_resfile.h"
#include "../include/HoeResource/resmgr.h"

// LinkedFile
HoeRes::LinkedFile::LinkedFile(int n, const tchar* filename) 
	: m_index(n), m_filename(filename), 
	  m_g(INVALID_HANDLE_VALUE), m_ptr(NULL) 
{
}

HoeRes::LinkedFile::~LinkedFile()
{
	if (m_g != INVALID_HANDLE_VALUE)
		FreeResource(m_g);
}

byte * HoeRes::LinkedFile::Get(HINSTANCE inst)
{
	if (m_ptr) return m_ptr;
	HRSRC sr = FindResource(inst,m_filename,T("HOERES"));
	//DWORD dw = SizeofResource(hInstance,sr);
	m_g = LoadResource(inst,sr);
	return m_ptr = (byte*)LockResource(m_g);
}

// LinkedReader
HoeRes::LinkedReader::LinkedReader(byte * ptr) : m_ptr(ptr) 
{
}

size_t HoeRes::LinkedReader::Read(void* ptr, size_t size)
{
	memcpy(ptr, m_ptr, size);
	return size;
}

uint HoeRes::LinkedReader::Close()
{
	delete this;
	return 0;
}

// LinkedResourceMgr

HoeRes::LinkedResourceMgr::LinkedResourceMgr(HINSTANCE inst, 
		HoeRes::SymbolLink* sym,
		const tchar * files[]) 
	: m_inst(inst), m_sym(sym), m_files(files)
{
}

HoeCore::ReadStream* 
	HoeRes::LinkedResourceMgr::GetResource(const tchar* name, const tchar** ns)
{
	// find symbol

	return LoadSymbol(m_sym[10]);
}

HoeCore::ReadStream * HoeRes::LinkedResourceMgr::LoadSymbol(HoeRes::SymbolLink& sym)
{
	LinkedFile& lf = GetFile(sym.fn);
	byte * ptr = lf.Get(m_inst);
	if (!ptr) return 0;
	return new LinkedReader(ptr+sym.pos);
}

HoeRes::LinkedFile& HoeRes::LinkedResourceMgr::GetFile(int n)
{
	for (HoeRes::LinkedFileList::Iterator i(m_openfiles);i;i++)
	{
		if (i->GetIndex() == n)
			return *i;
	}
	return m_openfiles.Add(LinkedFile(n, m_files[n]));
}



