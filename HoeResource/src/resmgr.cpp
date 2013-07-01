
#include "StdAfx.h"
#include "../include/HoeResource/hoe_resfile.h"
#include "../include/HoeResource/resmgr.h"

#ifdef _WIN32
// LinkedFile
HoeRes::LinkedFile::LinkedFile(int n, const HoeRes::SymbolFile* sym) 
	: m_index(n), m_sym(sym), 
	  m_g(INVALID_HANDLE_VALUE), m_ptr(NULL) 
{
}

HoeRes::LinkedFile::~LinkedFile()
{
#ifdef _WIN32_WINNT
	if (m_g != INVALID_HANDLE_VALUE)
		FreeResource(m_g);
#endif
}

byte * HoeRes::LinkedFile::Get(HINSTANCE inst)
{
	if (m_ptr) return m_ptr;
	HRSRC sr = FindResource(inst,m_sym->name,T("HOERES"));
	DWORD dw = SizeofResource(inst,sr);
	hoe_assert(dw == m_sym->size);
	m_g = LoadResource(inst,sr);
	m_ptr = (byte*)LockResource(m_g);
	// compute adler
	unsigned long a = adler32(0L, NULL, 0L);
	a = adler32(a, (Bytef*)m_ptr, dw);
	hoe_assert(a == m_sym->adler);
	return m_ptr;
}

// LinkedReader
HoeRes::LinkedReader::LinkedReader(byte * ptr) : m_ptr(ptr) 
{
}

size_t HoeRes::LinkedReader::Read(void* ptr, size_t size)
{
	memcpy(ptr, m_ptr, size);
	m_ptr += size;
	return size;
}

uint HoeRes::LinkedReader::Close()
{
	delete this;
	return 0;
}

// LinkedResourceMgr

HoeRes::LinkedResourceMgr::LinkedResourceMgr(HINSTANCE inst, 
		const HoeRes::SymbolLink* sym,
		const HoeRes::SymbolFile* files) 
	: m_inst(inst), m_sym(sym), m_files(files)
{
}

HoeCore::ReadStream* 
	HoeRes::LinkedResourceMgr::GetResource(const tchar* name, const tchar** ns)
{
	for (const HoeRes::SymbolLink* s=m_sym;s->name;s++)
	{
		if (HoeCore::string::cmp(s->name, name) == 0)
		{
			return LoadSymbol(*s);
		}
	}
	// find symbol
	return NULL;
}

HoeCore::ReadStream * HoeRes::LinkedResourceMgr::LoadSymbol(const HoeRes::SymbolLink& sym)
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
	return m_openfiles.Add(LinkedFile(n, &m_files[n]));
}

#endif

