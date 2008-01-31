
#include "StdAfx.h"
#include "../include/HoeCore/hoe_file.h"

HoeCore::File::~File()
{
	uint ref = Close();
	hoe_assert(ref == 0 && "Some streams opended.");
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
		acces = GENERIC_WRITE|GENERIC_READ;
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
	case hftRead:
		m = "rb";
		break;
	case hftWrite:
	case hftAppend:
	case hftTemp:
	case hftRewrite:
		m = "wb";
		break;
	default:
		not_implemented("file open modes");
	};

	m_file = fopen(m_name,m);
	if (m_file != InvalidHandle)
		opended = true;
#endif
	m_refs = 1;
	m_pos = 0;
	return opended;
}

size_t HoeCore::File::Read(void* ptr, size_t size)
{
	if (!Open())
		return 0;
	size_t readed;
#ifdef WINDOWS_FILE_FUNC
	DWORD dwRead;
	ReadFile(m_file,ptr,(dword)size,&dwRead,NULL);
	readed = (size_t)dwRead;
#else
	readed = fread(ptr,1,size,m_file);
#endif
	m_pos += readed;
	return readed;
}

size_t HoeCore::File::Write(const void* ptr, size_t size)
{
	if (!Open() || m_mode == File::hftRead)
		return 0;
	size_t writed;
#ifdef WINDOWS_FILE_FUNC
	DWORD dwWrite;
	WriteFile(m_file,ptr,(DWORD)size,&dwWrite,NULL);
	writed = (size_t)dwWrite;
#else
	writed = fwrite(ptr,1,size,m_file);
#endif
	m_pos += writed;
	return writed;
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
	if (!IsOpen() || m_pos == ptr)
		return;
#ifdef WINDOWS_FILE_FUNC
	SetFilePointer(m_file,(long)ptr,0,FILE_BEGIN);
#else
	fseek(m_file,(long)ptr,SEEK_SET);
#endif
	m_pos = ptr;
}

size_t HoeCore::File::Tell() const
{
	size_t pos;
#ifdef WINDOWS_FILE_FUNC
	pos = SetFilePointer(m_file,0,0,FILE_CURRENT);
#else
	pos = ftell(m_file);
#endif
	hoe_assert(m_pos == pos);
	return pos;
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
	m_pos += size;
	return true;
}

void HoeCore::File::Flush()
{
	if (!IsOpen())
		return;
#ifdef WINDOWS_FILE_FUNC
	FlushFileBuffers(m_file);
#else
	fflush(m_file);
#endif
}

void HoeCore::File::Reset()
{
	Seek(0);
}

uint HoeCore::File::Close()
{
	if (!IsOpen())
		return 0;
	
	if (m_refs > 1)
		return --m_refs;

	// write buffers
	for (uint i=0;i < m_buffs.Count();i++)
	{
		Buffer& b = m_buffs[i];
		if (b.ptr && b.size)
		{
			Seek(b.tell);
			Write(b.ptr, b.size);
		}
	}
	m_buffs.Delete();

#ifdef WINDOWS_FILE_FUNC
	CloseHandle(m_file);
#else
	fclose(m_file);
#endif
	m_file = InvalidHandle;

	return 0;
}

void * HoeCore::File::CreateBuffer(size_t s)
{
	Buffer& b = m_buffs.Add();
	b.tell = Tell();
	CreateSpace(s);
	b.Alloc(s);
	return b.ptr;
}

HoeCore::ReadStream* HoeCore::File::CreateReader(size_t pos)
{
	if (!Open())
		return NULL;
	m_refs++;
	return new FileReader(*this, pos, true);
}

HoeCore::FileReader::FileReader(File& file, size_t pos, bool destroy)
	: m_file(file), m_pos(pos), m_destroyself(destroy), m_open(true)
{
	
}

size_t HoeCore::FileReader::Read(void* ptr, size_t size)
{
	m_file.Seek(m_pos);
	size_t r = m_file.Read(ptr, size);
	m_pos += r;
	return r;
}

uint HoeCore::FileReader::Close()
{
	uint r = m_open && m_file.IsOpen() ? m_file.Close():0;
	m_open = false;
	if (m_destroyself)
		delete this;
	return r;
}

HoeCore::ReadStream* HoeCore::FileReader::CreateReader(size_t pos)
{
	return m_file.CreateReader(pos);
}

// file utils
bool HoeCore::SetRootDir(const tchar * dir)
{
#ifdef _WIN32
	return (SetCurrentDirectory(dir) != FALSE);
#endif
#ifdef _LINUX
	return (chdir(dir) == 0);
#endif
	return false;
}

const tchar * HoeCore::GetBaseDir(const tchar * path)
{
	static tchar basedir[ 1024 ];
	size_t j;

	HoeCore::string::copy( basedir, path, 1024 );

	for (j=HoeCore::string::len(basedir)-1;j > 0;j--)
	{
 		if (basedir[j] == '/' || basedir[j] == '\\')
		{
			basedir[j] = '\0';
			return basedir;
		}
	}

	return T(".");
}




