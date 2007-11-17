
#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus

#include <string>

namespace HoeUtils {

inline char pathcmp(char c1, char c2)
{
#ifdef _WIN32 
	if (c1 >= 'a' && c1 <= 'z')
		c1 -= 'a' - 'A';
	if (c2 >= 'a' && c2 <= 'z')
		c2 -= 'a' - 'A';
#endif
	return c1 == c2;
}

//FILE * log_out = stdout;

void print(const char * szFormat,...);

void fthrow(const char * szFormat,...);

bool Run(char * szCmd);

bool SysCmd(char * szCmd,int timeout);

void change_ext(char * fname,const char * ext);
bool is_ext(const char * fname,const char *ext);
bool is_absolutpath(const char * path);

std::string GetFileName(const std::string path);
std::string GetFileTitle(const std::string path);
std::string GetFileDir(const std::string path);
std::string fixpath(const std::string path);
std::string relativepath(const std::string main, const std::string path);
std::string fullpath(const std::string main, const std::string relpath);

bool is_file(const char * name);

int unpackargv(char * a,char *argv[]);

int next_max(int max,int);

class IDGEN
{
	static int sid;
	int id;
public:
	IDGEN();
	int GetID();
	operator int () { return (int)GetID(); }
};

class Stream
{
public:
	virtual unsigned long GetPos() = 0;
	virtual void SetPos(int pos) = 0;
	virtual bool Write(const void * lpBuffer,size_t size) = 0;
	virtual bool SkipZero(size_t size) = 0;
	virtual int CreateSpace(size_t size) = 0;
	virtual bool Read(void * lpBuffer,size_t size) = 0;
	virtual bool Read(void * lpBuffer,size_t size, size_t * bytes_read) = 0;
};

class File : public Stream
{

private:
#ifdef _WIN32
	void * handle;
#else
	FILE * file;
#endif
public:
	enum Mode
	{
		mRead,
		mWrite
	};
	File();
	~File();
	bool Open(const char * name,Mode m);
	virtual unsigned long GetPos();
	virtual void SetPos(int pos);
	virtual bool Write(const void * lpBuffer,size_t size);
	virtual bool SkipZero(size_t size);
	virtual int CreateSpace(size_t size);
	virtual bool Read(void * lpBuffer,size_t size);
	virtual bool Read(void * lpBuffer,size_t size, size_t * bytes_read);
	bool CopyToStream(Stream * stream, size_t size);
	int GetSize();
	void Close();
    bool IfOpen();
};

#define FILE_BUFFERED_BUFFER_SIZE 512

class FileRBuffered : public File
{
	char buffer[FILE_BUFFERED_BUFFER_SIZE];
	int bsize;
public:
	int GetChar();
	bool Open(const char * name);
};

class FileRText : public FileRBuffered
{
public:
	int ReadLine(char * buff, int maxsize);
};

namespace nums
{
    enum type
	{
		Float,
		Hex,
		Num
	};

	type GetType(const char * p);
	float GetFloat(const char * p);
	unsigned long GetHex(const char * p);
	int GetNum(const char * p);

}

class DataBuffer
{
	int m_size_per;
	void * m_data;
	int m_num;
	int m_max;
public:
	DataBuffer(int sizeofelement);
	~DataBuffer();
	int AddData(void * data);
	void * Get(int index);

	bool Empty() { return m_num == 0; }
	void Delete(int n);
	int GetNum() { return m_num;}
	int GetMax() { return m_max;}
	int GetSize() { return m_num * m_size_per;}
	void * GetData() { return m_data;}
	void Clean();
};

template <class CLASS>
class List : public DataBuffer
{
public:
	List() : DataBuffer(sizeof(CLASS))
    {
    }
	int Add(CLASS &c)
    {
		return AddData(&c);
    }
	CLASS & operator[](const int index)
	{
		return *((CLASS*) Get(index)); 
	}
};

class XMLparser
{
	File m_file;
protected:
public:
	XMLparser();
	~XMLparser();
	bool parse(const char * fname);
};



}; // namespace HoeUtils

bool HoeUtilsTest();

extern "C" {
#endif // __cplusplus

float hu_GetFloat(const char * p);
unsigned long hu_GetHex(const char * p);
int hu_GetNum(const char * p);
const char * hu_GetString(const char * s);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _UTILS_H_
