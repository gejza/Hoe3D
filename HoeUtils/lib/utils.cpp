
#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

#include "hoeUtils.h"

namespace HoeUtils {

//FILE * log_out = stdout;
#ifndef _LINUX
void print(const char * szFormat,...)
{
	va_list args;

	va_start(args, szFormat);
	vfprintf(stdout,szFormat, args );
	va_end(args);
}
#endif

void fthrow(const char * szFormat,...)
{
	va_list args;
	static char buff[512];

	va_start(args, szFormat);
#ifdef _MSC_VER
	_vsnprintf(buff,512,szFormat, args );
#else
	vsnprintf(buff,512,szFormat, args );
#endif
	va_end(args);

	throw (const char *)buff;
}

bool Run(char * szCmd, int timeout)
{
#ifdef _WIN32
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = SW_NORMAL;

	if (!CreateProcess(NULL,szCmd,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi))
	{
		print("Failed create process. ERR:%d\n",GetLastError());
		return false;
	}

	do {
		DWORD r = WaitForSingleObject(pi.hProcess,(timeout == 0) ? 10000:timeout);
		if (timeout)
		{
            if (r == WAIT_TIMEOUT)
			{
				printf("Time out.");
				return false;
			}
			else
				return true;
		}
	}
	while (timeout);
#endif
	return true;
}

bool SysCmd(char * szCmd,int timeout)
{
	char cmd[256];
	sprintf(cmd,"cmd.exe /c \"%s\"",szCmd);
	return Run(cmd,timeout);
}

std::string GetFileName(const std::string path)
{
	int f1 = (int)path.find_last_of('/');
	int f2 = (int)path.find_last_of('\\');

	int f = (f1 > f2) ? f1:f2;
	
	return path.substr(f+1);
}

std::string GetFileTitle(const std::string path)
{
	std::string title = GetFileName(path);
	int f = (int)title.find_last_of('.');

	if (f == -1)
		return title;
	else
		return title.substr(0,f);
}

std::string GetFileDir(const std::string path)
{
	int f1 = (int)path.find_last_of('/');
	int f2 = (int)path.find_last_of('\\');

	int f = (f1 > f2) ? f1:f2;
	
	return path.substr(0,f+1);
}

std::string fixpath(const std::string path)
{
	char ap[3] = {0};
	std::string np = path;
	int ib = -1;
#ifdef _WIN32
	while ((ib = np.find("\\",ib+1)) != -1)
	{
		np = np.replace(ib,1,"/");
	}	
#endif
	// projede to
	while ((ib = np.find("//")) != -1)
	{
		np = np.substr(0,ib) + np.substr(ib+1);
	}
	while ((ib = np.find("/./")) != -1)
	{
		np = np.substr(0,ib) + np.substr(ib+2);
	}
	ib = -1;
	while ((ib = np.find("/../",ib+1)) != -1)
	{
		if (ib > 2 && np.compare(ib-3,4,"/../") != 0)
		{
			int fr = np.find_last_of("/",ib-1);
			std::string f;
			if (fr > 0)
				f = np.substr(0,fr);
			np = f+np.substr(ib+3);
			ib = 0;
		}
	}

	// upravit konec
	ib = np.find_last_of("/");
	if (ib != -1)
	{
		if (np.compare(ib,2,"/") == 0)
		{
			np.resize(np.length()-1);
		}
		else if (np.compare(ib,3,"/.") == 0)
		{
			np.resize(np.length()-2);
		}
	}

	return np;
}

std::string relativepath(const std::string dir, const std::string path)
{
	int i;
	std::string ndir = fixpath(dir);
	std::string npath = fixpath(path);
	// ted pojedu a budu zjistovat kam az jsou adresarove stejny
	if (!pathcmp(ndir[0],npath[0]))
		return npath;
	
	int firstlen = 0;
	for (i = 0;;i++)
	{
		if (ndir[i] == '\0')
		{
			if (npath[i] == '/')
				firstlen = -1;
			break;
		}
		// chyba, vrati se opravena puvodni cesta
		if (npath[i] == '\0')
			return npath;
		if (!pathcmp(ndir[i],npath[i]))
			break;
		if (ndir[i] == '/')
			firstlen = i+1;
	}
	if (firstlen == -1)
	{
		npath = npath.substr(ndir.length()+1);
		ndir = "";
	}
	else
	{
		npath = npath.substr(firstlen);
		ndir = ndir.substr(firstlen);
	}
	int numbackdir = 0;
	if (!ndir.empty())
	{
		numbackdir++;
		// spocitat kolik dir obsahuje adresaru
		for (std::string::const_iterator it=ndir.begin();it!=ndir.end();it++)
			if (*it == '/')
				numbackdir++;
	}

	// poskladani cesty
	std::string finpath;
	for (i=0;i < numbackdir;i++)
	{
		finpath.append("../");
	}
	finpath.append(npath);
	return finpath;
}

std::string fullpath(const std::string dir, const std::string relpath)
{
	if (is_absolutpath(relpath.c_str()))
		return fixpath(relpath);
	return fixpath(fixpath(dir).append("/").append(fixpath(relpath)));
}

void change_ext(char * fname,const char * ext)
{
	char * p = fname;
	while (*p) p++;
	while (p > fname && *p != '\\' && *p != '/' && *p != '.') p--;
	if (*p == '.')
		*p = '\0';
	strcat(fname,".");
	strcat(fname,ext);
}

#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? c - 'a' + 'A':c)

bool is_ext(const char * fname,const char *ext)
{
	const char * p = fname;
	const char * e = ext;
	while (*p) p++;
	while (*e) e++;

	while (p >= fname && e >= ext)
		if (TO_UPPER(*p) == TO_UPPER(*e))
		{
			p--;
			e--;
		}
		else
			return false;
	return true;
}

bool is_absolutpath(const char * path)
{
#ifdef _WIN32
	return (path[0] && path[1] == ':' && (path[2] == '/' || path[2] == '\\'));
#else
	return (path[0] == '/');
#endif

}

bool is_file(const char * name)
{
	FILE * f = fopen(name,"r");
	if (!f)
		return false;
	fclose(f);
	return true;
}

int unpackargv(char * a,char *argv[])
{
	char *ptr = a;
    int num = 0;
	while (*ptr)
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') ptr++;
		if (*ptr)
			argv[num++] = ptr;
		while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r') ptr++;
		if (*ptr) *ptr++ = '\0';
	}
	return num;
}

int next_max(int max,int)
{
	if (max < 1000)
        return max + 100;
	if (max < 10000)
		return max + 1000;
	return (max + (max / 10));
}

int IDGEN::sid = 1;

IDGEN::IDGEN()
{
	id = -1;
}

int IDGEN::GetID()
{
	if (id == -1)
		id = sid++;
	return id;
}

File::File()
{
#ifdef _WIN32
	handle = INVALID_HANDLE_VALUE;
#else
	file = NULL;
#endif
}

File::~File()
{
        Close();
}

bool File::Open(const char * name,Mode m)
{
#ifdef _WIN32
	if (m == mRead)
		handle = CreateFile(name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,NULL);
	if (m == mWrite)
		handle = CreateFile(name,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,NULL);


	return (handle != INVALID_HANDLE_VALUE);
#else
	file = fopen(name,(m == mRead) ? "r":"w");
	return (file != NULL);
#endif
}

bool File::IfOpen()
{
#ifdef _WIN32
	return (handle != INVALID_HANDLE_VALUE);
#else
	return (file != NULL);
#endif
}

unsigned long File::GetPos()
{
#ifdef _WIN32
	return SetFilePointer(handle,0,0,FILE_CURRENT);
#else
	return ftell(file);
#endif
}
void File::SetPos(int pos)
{
#ifdef _WIN32
	SetFilePointer(handle,pos,0,FILE_BEGIN);
#else
	fseek(file,pos,SEEK_SET);
#endif
}

bool File::Write(const void * lpBuffer,size_t size)
{
#ifdef _WIN32
	DWORD dwWrite;
	WriteFile(handle,lpBuffer,(DWORD)size,&dwWrite,NULL);
	return (size == dwWrite);
#else
	return (fwrite(lpBuffer,1,size,file) == size);
#endif
}

bool File::SkipZero(size_t size)
{
	char buff[512] = {0};
	while (size > 0)
	{
		size_t act = (size > sizeof(buff)) ? sizeof(buff):size;
		if (!Write(buff,act))
			return false;

		size -= act;
	}
	return true;
}

int File::CreateSpace(size_t size)
{
	unsigned long pos = this->GetPos();
	if (SkipZero(size) == false)
		return -1;
	else
		return pos;
}

bool File::Read(void * lpBuffer,size_t size, size_t * bytes_read)
{
#ifdef _WIN32
	DWORD dwRead;
	ReadFile(handle,lpBuffer,(DWORD)size,&dwRead,NULL);
	*bytes_read = dwRead;
	return (size == dwRead);
#else
	return ((*bytes_read = fread(lpBuffer,1,size,file))== size);
#endif

}

bool File::Read(void * lpBuffer,size_t size)
{
	return Read(lpBuffer,size,&size);
}

bool File::CopyToStream(Stream * stream, size_t size)
{
	char buff[1000];
	while (size > 0)
	{
		int min = (size > sizeof(buff)) ? sizeof(buff):size;
		size -= min;
		if (!Read(buff,min))
			return false;
		if (!stream->Write(buff,min))
			return false;
	}
	return true;
}

int File::GetSize()
{
#ifdef _WIN32
	return GetFileSize(handle,NULL);
#else
        long fpos = ftell(file);
        fseek(file,0,SEEK_END);
        long size = ftell(file);
        fseek(file,fpos,SEEK_SET);
        return size;
#endif
}

void File::Close()
{
#ifdef _WIN32
        if (handle != INVALID_HANDLE_VALUE)
                CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
#else
        if (file)
                fclose(file);
        file = NULL;
#endif
}

bool FileRBuffered::Open(const char * name)
{
	bsize = 0;
	return File::Open(name,File::mRead);
}

int FileRText::ReadLine(char * buff, int maxlen)
{
	int readchars = 0;

	while (readchars < (maxlen-1))
	{
		int c = this->GetChar();

		if (c == EOF)
		{
			if (readchars == 0)
				return -1;
			else
			{
				buff[readchars] = '\0';
				return 1;
			}
		}

		if (c == '\n')
		{
				buff[readchars] = '\0';
				return 1;
		}

		if (c >= ' ')
		{
			buff[readchars] = (char)c;
			readchars++;
		}
	}
	  	
	buff[readchars] = '\0';

	return 0;
}

namespace nums
{

	type GetType(const char * p)
	{
		assert(*p);
		if (p[0] == '0' && p[1] == 'x')
			return Hex;
		
		const char *pp = p;
		while (*pp)
		{
			if (*pp == '.')
				return Float;
			pp++;
		}
		if (*(--pp) == 'f')
			return Float;

		return Num;
	}

	float GetFloat(const char * p)
	{
		float num = 0.f;
		double des = 0.1f;
		float sign = 1.0f;
		if (*p == '+')
		{
			sign = 1.0f;p++;
		}
		else if (*p == '-')
		{
			sign = -1.0f;p++;
		}

		//
		while (*p != '.')
		{
			if (*p == '\0' || (*p == 'f' && p[1] == '\0'))
				return sign * num;
			if (*p >= '0' && *p <= '9')
			{
				num = num * 10 + (*p - '0');
				p++;
			}
			else
				throw "unrecognized character";
		}
		p++; // tecka
		while (!(*p == '\0' || (*p == 'f' && p[1] == '\0')))
		{
			if (*p >= '0' && *p <= '9')
			{
				num = num + float(des * float(*p - '0'));
				des = des * 0.1f;
				p++;
			}
			else
				throw "unrecognized character";
		}
		
		return sign * num;
	}

	unsigned long GetHex(const char * p)
	{
		unsigned long num = 0x0;
		if (*p++ != '0' || *p++ != 'x')
			throw "error parse hex";
		do 
		{
			if (*p >= '0' && *p <= '9')
				num = num * 0x10 + (*p - '0');
			else if (*p >= 'a' && *p <= 'f')
				num = num * 0x10 + (*p - 'a' + 10);
			else if (*p >= 'A' && *p <= 'F')
				num = num * 0x10 + (*p - 'A' + 10);
			else
				throw "unrecognized character";

			p++;
		}
		while (*p);

		return num;
	}

	int GetNum(const char * p)
	{
		int num = 0;
		int sign = 1;
		if (*p == '+')
		{
			sign = 1;p++;
		}
		else if (*p == '-')
		{
			sign = -1;p++;
		}

		do 
		{
			if (*p >= '0' && *p <= '9')
				num = num * 10 + (*p - '0');
			else
				throw "unrecognized character";

			p++;
		}
		while (*p);

		return num * sign;
	}

}

///////// data buffer
DataBuffer::DataBuffer(int sizeofelement)
{
	m_size_per = sizeofelement;
	m_num = m_max = 0;
	m_data = NULL;
}
DataBuffer::~DataBuffer()
{
	if (m_data)
        free(m_data);
}
int DataBuffer::AddData(void * data)
{
	if (m_num >= m_max)
	{
		m_max = next_max(m_max,m_size_per);
		m_data = realloc(m_data,m_max * m_size_per);
	}
	memcpy((unsigned char*)m_data + (m_size_per*m_num),data,m_size_per);
	return m_num++;
}

void * DataBuffer::Get(int index)
{
	return (unsigned char*)m_data + (index * m_size_per);
}

void DataBuffer::Delete(int n)
{
	assert(n >= 0 && n < m_num);
	for (n++;n < m_num;n++)
		memcpy((unsigned char*)m_data + ((n-1) * m_size_per),(unsigned char*)m_data + (n * m_size_per),m_size_per);
	m_num--;
}

void DataBuffer::Clean()
{
	free(m_data);
	m_data = NULL;
	m_num = 0;
}


}; // namespace HoeUtils

///////////////////////////////

extern "C" {

float hu_GetFloat(const char * p)
{
	return HoeUtils::nums::GetFloat(p);
}
unsigned long hu_GetHex(const char * p)
{
	return HoeUtils::nums::GetHex(p);
}

int hu_GetNum(const char * p)
{
	return HoeUtils::nums::GetNum(p);
}

const char * hu_GetString(const char * s)
{
    static char buff[512];
    strcpy(buff, s);
	buff[strlen(buff)-1] = '\0';
    return (buff + 1);
}

}

/////////////////////////////////


using namespace HoeUtils;

static bool NumsTest()
{
	if (nums::GetType("-45.6f") != nums::Float || nums::GetFloat("-45.6f") != -45.6f)
	{
		printf("float test error in utils.cpp\n");
		return false;
	}

	if (nums::GetType("0xff12fe") != nums::Hex || nums::GetHex("0xff12fe") != 0xff12fe)
	{
		printf("hex test error in utils.cpp\n");
		return false;
	}

	if (nums::GetType("-123") != nums::Num || nums::GetNum("-123") != -123)
	{
		printf("num test error in utils.cpp\n");
		return false;
	}

	return true;
}

static bool PathTest()
{
	if (fixpath("c:\\adresar1\\nov\\novalid\\..\\..\\.\\\\adresar2\\") != "c:/adresar1/adresar2")
	{
		printf("fixpath() test error in utils.cpp\n");
		return false;
	}

	if (relativepath("c:\\adr1\\adr2\\adr3\\","C:/ADr1/adr4/soubor.txt") != "../../adr4/soubor.txt")
	{
		printf("relativepath() test error in utils.cpp\n");
		return false;
	}

	if (relativepath("c:\\adr1\\","C:/ADr1/adr2/soubor.txt") != "adr2/soubor.txt")
	{
		printf("relativepath() test error in utils.cpp\n");
		return false;
	}
	if (fullpath("c:\\adr1\\adr2\\adr3\\","../../adr4/soubor.txt") != "c:/adr1/adr4/soubor.txt")
	{
		printf("fullpath() test error in utils.cpp\n");
		return false;
	}

	return true;
}

bool HoeUtilsTest()
{
	if (!NumsTest())
		return false;
	if (!PathTest())
		return false;
	return true;
}




int FileRBuffered::GetChar()
{
	char c;
	if (this->Read(&c,1))
		return c;
	else
		return EOF;
}
