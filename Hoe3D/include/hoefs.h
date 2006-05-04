
/**
   @file    hoefs.h
   @brief   Interface pro souborovy system
*/

#ifndef _HOE_FILE_SYSTEM_H_
#define _HOE_FILE_SYSTEM_H_

#include "hoetypes.h"
#include "hoeinterfaces.h"

/// mod pro otevirani souboru
enum EHoeFileMode
{
	hftRead,
	hftWrite,
};

class HoeFileReader;

/** popis souboru */
struct THoeFileDesc
{
	int fileid; ///< id souboru
	const char * name; ///< jmeno souboru
	uint resid; ///< id resource souboru
	uint version; ///< verze resource souboru
};

/**
 * Interface souboru
 */
class XHoeFile
{
public:
	/// vraci id souboru pokud je zaregistrovan v systemu jinak vraci -1
	virtual uint GetID() = 0;
	/// znovuotevre soubor
	virtual bool ReOpen() = 0;
	/// vrati true pokud je soubor otevreny
	virtual bool IsOpen() = 0;
	/// nastavi pozici v souboru na 0
	virtual void Reset() = 0;
	/// zavre soubor
	virtual void Close() = 0;
	/// vrati jmeno souboru, pokud je nezname vraci NULL
	virtual const char * GetFileName() = 0;
	/// nastaveni aktualni pozice
	virtual void Seek(size_t ptr) = 0;
	/// vraci aktualni pozici v souboru
	virtual size_t Tell() = 0;
	/// preskoceni aktualni pozice, pokud je soubor otevren pro zapis zapise same 0
	virtual void Skip(int ptr) = 0;
	/// cteni ze souboru
	virtual bool Read(void * buff,size_t size) = 0;
	/// zapis do souboru
	virtual bool Write(const void * buff,size_t size) = 0;
	/// nastaveni pozice podle chunku (pokud existuje)
	virtual HoeFileReader FindChunk(uint id) = 0;
	/// ziskani informaci o souboru
	virtual void GetDesc(THoeFileDesc * desc) = 0;
};

class HoeFileReader
{
	XHoeFile * m_file;
	size_t m_startpos;
	size_t m_pos;
public:
	HoeFileReader()
	{
		m_file = NULL;m_startpos=m_pos=0;
	}
	HoeFileReader(const HoeFileReader & r)
	{
		m_file=r.m_file; m_startpos=m_pos=r.m_pos;
	}
	HoeFileReader(XHoeFile * f, size_t start)
	{
		m_file = f;m_startpos=m_pos=start;
	}
	bool operator !()
	{
		return m_file == NULL;
	}
	XHoeFile * GetFile() { return m_file;}
	bool Read(void * buff,size_t size)
	{
		if (!m_file) return false;
		m_file->Seek(m_pos);
		if (!m_file->Read(buff,size))
			return false;
		m_pos += size;
		return true;
	}
	template<class T> T Read()
	{
		T t;
		Read(&t, sizeof(T));
		return t;
	}
    bool Skip(size_t size)
	{
		m_pos += size;return true;
	}
	bool IsValid() { return (m_file && m_file->IsOpen()); }
	size_t GetReadSize() { return m_pos - m_startpos; }
	void Reset() { m_pos = m_startpos; }
};

class HoeFileWriter
{
	XHoeFile * m_file;
	size_t m_startpos;
	size_t m_size;
public:
	HoeFileWriter()
	{
		m_file = NULL;
	}
	HoeFileWriter(const HoeFileWriter & w)
	{
		m_file=w.m_file;
	}
	HoeFileWriter(XHoeFile * f)
	{
		m_file = f;
	}
	bool operator !()
	{
		return m_file == NULL;
	}
	XHoeFile * GetFile() { return m_file;}
	bool Write(const void * buff,size_t size)
	{
		if (!m_file) return false;
		if (!m_file->Write(buff,size))
			return false;
		return true;
	}
	template<class T> bool Write(T t)
	{
		return Write(&t, sizeof(T));
	}
};

class HoeFileAutoWriter
{
	XHoeFile * m_file;
	size_t m_startpos;
	size_t m_size;
	void * m_p;
	HoeFileAutoWriter(const HoeFileAutoWriter & r)
	{
		//m_file=r.m_file; m_startpos=m_pos=r.m_pos;
	}
public:
	HoeFileAutoWriter()
	{
		m_file = NULL;
	}

	HoeFileAutoWriter(XHoeFile * f, void * p, size_t size)
	{
		m_file = f;m_startpos=f->Tell();
		m_p = p; m_size = size;
		m_file->Skip((dword)size);
	}
	//HoeFileWriter(XHoeFile * f, dword start, dword size)
	//{
	//	m_file = f;m_startpos=m_pos=start;
	//}
	bool operator !()
	{
		return m_file == NULL;
	}
	XHoeFile * GetFile() { return m_file;}
	bool Flush()
	{
		size_t pos = m_file->Tell();
		m_file->Seek(m_startpos);
		m_file->Write(m_p, m_size);
		m_file->Seek(pos);
		return true;
	}
};

class XHoeFS
{
public:
	virtual XHoeFile * Open(const char *fname,EHoeFileMode mode = hftRead) = 0;
	virtual uint AddResourceFile(const char *) = 0;

	virtual HoeFileReader FindResource_(const char * name,dword type = 0) = 0;
	virtual XHoeFile * GetFile(uint id) = 0;

	virtual void CloseAll() = 0;
	virtual void PrintList() = 0;
};

class XHoeConsole;

typedef XHoeFS * (HOEAPI *HOEFS_FUNCCREATE)(int ver, XHoeConsole * con);


#endif // _HOE_FILE_SYSTEM_H_

