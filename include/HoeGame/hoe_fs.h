
#ifndef _BASE_HOE_FS_H_
#define _BASE_HOE_FS_H_

#include <hoefs.h>
#include "hoe_game.h"

namespace HoeGame {

#ifdef _WIN32
typedef HANDLE HoeFileHandle;
#else
typedef FILE * HoeFileHandle;
#endif

// novy filesystem
// presunout do hoe core
// tm file
// stream write a stream reader
// resource manager pro nacitani z hoe3d

class BaseConsole;

class HoeFile : public XHoeFile
{
private:
	static uint idcounter;
	char * name;
	uint m_id; 
	dword type;
	bool isopen;
	HoeFile * next;
	EHoeFileMode m_mode;
//	HoeLog *m_log;
private:
	HoeFileHandle file;
	bool Opended();
public:
	HoeFile();
	//HoeFile(HoeLog *log);
	~HoeFile();

	virtual bool ReOpen();
	bool Open(const char *fname,EHoeFileMode mode = hftRead);
	bool IsOpen() { return isopen;}
	uint GetID() { return m_id;}
	virtual size_t Read(void * buff,const size_t size);
	virtual bool Write(const void * buff, const size_t size);

	void Seek(size_t ptr);
	virtual size_t Tell();
	virtual bool Skip(size_t size);
	void Reset();
	void Close();
	const char * GetFileName();
	virtual HoeFileReader FindChunk(uint id);
	virtual void GetDesc(THoeFileDesc * desc);

	static const char * GetOpenFunc();

	friend class FileSystem;
};

class FileSystem : public XHoeFS
{
private:
	HoeFile * m_flist;
	//HoeFile * GetFile(char * name);
public:
	FileSystem();
	~FileSystem();
	virtual XHoeFile *  Open(const char *fname,EHoeFileMode mode = hftRead);

	virtual uint AddResourceFile(const char *);
	virtual HoeFileReader FindResource_(const tchar * name,dword type = 0);
	virtual XHoeFile * GetFile(uint id);

	virtual void CloseAll();
	virtual void PrintList();

};

class TextFile
{
protected:
	FILE * file;
public:
	TextFile();
	~TextFile();
	bool Open(const char * filename);
	void Close();
	inline bool IsOpen() { return file != NULL; }
	bool GetLine(char * buff, size_t size);
};

} // namespace HoeGame

#endif // _BASE_HOE_FS_H_

