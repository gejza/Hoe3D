
#include "StdAfx.h"
#include "../include/hoe_file.h"

HoeCore::File::~File()
{
	Close();
}

bool HoeCore::File::Open(const HoeCore::CString name, HoeCore::File::EHoeFileMode mode)
{
	m_mode = mode;
	m_name = name;
	if (name == "")
		return false;
	bool opended = false;
#ifdef WINDOWS_FILE_FUNC
	dword acces = 0;
	dword creating = 0;
	dword attr = 0;
	switch (mode)
	{
	default:
	case hftRead:
		acces = GENERIC_READ;
		creating = OPEN_EXISTING;
		break;
	case hftAppend:
	case hftWrite:
		acces = GENERIC_WRITE;
		creating = OPEN_ALWAYS;
		break;
	case hftTemp:
		attr = FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE;
	case hftRewrite:
		acces = GENERIC_WRITE;
		creating = CREATE_ALWAYS;
		break;
	};
	m_file = CreateFile(name,acces,FILE_SHARE_READ,
		NULL,creating,attr,INVALID_HANDLE_VALUE);
	if (m_file != InvalidHandle)
		opended = true;
	if (opended && m_mode == hftAppend)
	{
		this->Seek(GetFileSize(m_file,NULL));
	}
#else
	const char * m;
	switch (mode)
	{
	default:
	case hftRead:
		m = "rb";
		break;
	case hftWrite:
		m = "wb";
		break;
	default:
		not_implemented("file open modes");
	};

	m_file = fopen(m_name,m);
	if (m_file != InvalidHandle)
		opended = true;
#endif
	return opended;
}

size_t HoeCore::File::Read(void* ptr, size_t size)
{
	if (!Open())
		return 0;
#ifdef WINDOWS_FILE_FUNC
	DWORD dwRead;
	ReadFile(m_file,ptr,(dword)size,&dwRead,NULL);
	return (size_t)dwRead;
#else
	return fread(ptr,1,size,m_file);
#endif
}

size_t HoeCore::File::Write(const void* ptr, size_t size)
{
	if (!Open() || m_mode == File::hftRead)
		return 0;
#ifdef WINDOWS_FILE_FUNC
	DWORD dwWrite;
	WriteFile(m_file,ptr,(DWORD)size,&dwWrite,NULL);
	return (size_t)dwWrite;
#else
	return fwrite(ptr,1,size,m_file);
#endif
}

const char * HoeCore::File::GetOpenFunc()
{
#ifdef WINDOWS_FILE_FUNC
	return "CreateFile";
#else
	return "fopen";
#endif
}

bool HoeCore::File::Open()
{
	return IsOpen() || Open(m_name, m_mode);
}

void HoeCore::File::Seek(size_t ptr)
{
	if (!IsOpen())
		return;
#ifdef WINDOWS_FILE_FUNC
	SetFilePointer(m_file,(long)ptr,0,FILE_BEGIN);
#else
	fseek(m_file,(long)ptr,SEEK_SET);
#endif
}

size_t HoeCore::File::Tell() const
{
#ifdef WINDOWS_FILE_FUNC
	return SetFilePointer(m_file,0,0,FILE_CURRENT);
#else
	return ftell(m_file);
#endif
}

bool HoeCore::File::Skip(size_t size)
{
	if (!IsOpen())
		return false;
#ifdef WINDOWS_FILE_FUNC
	SetFilePointer(m_file,(long)size,0,FILE_CURRENT);
#else
	fseek(m_file,(long)size,SEEK_CUR);
#endif
	return true;
}

void HoeCore::File::Reset()
{
	Seek(0);
}

void HoeCore::File::Close()
{
	if (!IsOpen())
		return;
#ifdef WINDOWS_FILE_FUNC
	CloseHandle(m_file);
#else
	fclose(m_file);
#endif
	m_file = InvalidHandle;
}


