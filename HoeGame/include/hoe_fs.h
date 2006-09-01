
#ifndef _BASE_HOE_FS_H_
#define _BASE_HOE_FS_H_

#include <hoefs.h>
#include "hoe_game.h"

BEGIN_HOEGAME

#ifdef _WIN32
typedef HANDLE HoeFileHandle;
#else
typedef FILE * HoeFileHandle;
#endif

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
	virtual bool Read(void * buff,size_t size);
	virtual bool Write(const void * buff,size_t size);

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
	virtual HoeFileReader FindResource_(const char * name,dword type = 0);
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

END_HOEGAME

#endif // _BASE_HOE_FS_H_

