
#ifndef _HOE_ENGINE_FS_H_
#define _HOE_ENGINE_FS_H_

#include <hoe3d.h>
#include <hoefs.h>

namespace HoeEditor {

class EditorFile : public XHoeFile
{
	wxFile file;
	wxString path;
public:
	bool OpenRead(const wxString &p);
	bool OpenWrite(const wxString &p);
	/// vraci id souboru pokud je zaregistrovan v systemu jinak vraci -1
	virtual uint GetID();
	/// znovuotevre soubor
	virtual bool ReOpen();
	/// vrati true pokud je soubor otevreny
	virtual bool IsOpen();
	/// nastavi pozici v souboru na 0
	virtual void Reset();
	/// zavre soubor
	virtual void Close();
	/// vrati jmeno souboru, pokud je nezname vraci NULL
	virtual const char * GetFileName();
	/// nastaveni aktualni pozice
	virtual void Seek(size_t ptr);
	/// vrati akt. pozici
	virtual size_t Tell();
	/// preskoceni aktualni pozice, pokud je soubor otevren pro zapis zapise same 0
	virtual bool Skip(size_t ptr);
	/// cteni ze souboru
	virtual size_t Read(void * buff,const size_t size);
	/// zapis do souboru
	virtual bool Write(const void * buff,const size_t size);
	/// nastaveni pozice podle chunku (pokud existuje)
	virtual HoeFileReader FindChunk(uint id);
	/// ziskani informaci o souboru
	virtual void GetDesc(THoeFileDesc * desc);
};

class EditorFS : public XHoeFS
{
protected:
	/** funkce ktera se vola, pokud nejake resource chybi */
	virtual bool ReqResource(const char * name) { return false; }
public:
	virtual XHoeFile * Open(const char *fname,EHoeFileMode mode = hftRead);
	virtual uint AddResourceFile(const char *) { return -1; };

	virtual HoeFileReader FindResource_(const char * name,dword type = 0);
	virtual XHoeFile * GetFile(uint id) { return NULL; }

	virtual void CloseAll() {};
	virtual void PrintList() {};

	bool Init(const char * dllname);

	virtual uint GetFilesCount() const { return 0; }
	virtual XHoeFile * GetFileResource(int index) { return NULL; }
};

}; // namespace HoeEditor

#endif // _HOE_ENGINE_FS_H_

