
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "filesystem.h"
#include "../include/hoefs.h"

extern XHoeConsole * _Con;

/// HoeFileReader
/*HoeFileReader::HoeFileReader(XHoeFile * file, dword start)
{
	m_file = file;
	m_posstart = m_pos = start;
}

HoeFileReader::HoeFileReader(uint id, dword start)
{
	m_file = GetFileSystem()->GetFile(id);
	m_posstart = m_pos = start;
}

HoeFileReader::HoeFileReader(const HoeFileReader & reader)
{
	m_file = reader.m_file;
	m_posstart = m_pos = reader.m_pos;
}

void HoeFileReader::Reset()
{
	Open();
	m_pos = m_posstart;
}

bool HoeFileReader::Open()
{
	if ( m_file )
	{
		if (m_file->IsOpen())
			return true;

		return m_file->ReOpen();

	}
	return false;
}

bool HoeFileReader::Read(void * buff,dword size)
{
	if (!IsValid())
	{
		if (!Open())
			return false;
	}

	m_file->Seek(m_pos);

	if (!m_file->Read(buff,size))
		return false;

	m_pos += size;
	
	return true;
}

bool HoeFileReader::Skip(dword size)
{
	m_pos += size;
	return true;
}

bool FileSystem::Load(const char * dllname)
{
	HOEFS_FUNCCREATE GetFSInterface;
	
#ifdef _WIN32
	m_lib = LoadLibrary(dllname);
	if (!m_lib)
	{
		Con_Print("Failed load library: %s",dllname);
		return false;
	}

	GetFSInterface = (HOEFS_FUNCCREATE)GetProcAddress(m_lib,"_CreateFS@8");
	if (!GetFSInterface)
	{
		Con_Print("Found symbol _CreateFS@8 failed.");
		return false;
	}
#endif

#ifdef _LINUX

	m_lib = dlopen (dllname, RTLD_LAZY);
	if (!m_lib) {
		Con_Print("error: %s", dlerror());
		return false;
	}

	GetFSInterface =(HOEFS_FUNCCREATE) dlsym(m_lib, "CreateFS");
	if (!GetFSInterface)
	{
		Con_Print("error: %s",dlerror());
		return false;
	}
#endif

	XHoeFS * fs = GetFSInterface(HOEFSSDK_VERSION,_Con);
	shared::fs = fs; 

	//	UNSET_SHARED_PTR(fs);

	return (fs != NULL);
}

void FileSystem::SetUserFS(void * fs)
{
	shared::fs = (XHoeFS*)fs;
}

*/



