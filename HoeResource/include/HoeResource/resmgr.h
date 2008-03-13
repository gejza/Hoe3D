
#ifndef _HOERESOURCE_RESMGR_H_
#define _HOERESOURCE_RESMGR_H_

namespace HoeRes {

struct SymbolLink;

/** Interface pro resource manager */
class XResourceMgr
{
public:
	virtual HoeCore::ReadStream * GetResource(const tchar* name, const tchar** ns = NULL) = 0;
};

#ifdef _WIN32

class LinkedFile
{
	int m_index;
	const tchar* m_filename;
	HGLOBAL m_g;
	byte* m_ptr;
public:
	LinkedFile(int n, const tchar* filename);
	~LinkedFile();
	int GetIndex() { return m_index; }
	byte * Get(HINSTANCE inst);
};
typedef HoeCore::List<LinkedFile> LinkedFileList;

class LinkedReader : public HoeCore::ReadStream
{
	byte * m_ptr;
public:
	LinkedReader(byte * ptr);
	virtual size_t Read(void* ptr, size_t size);
	virtual uint Close();
	virtual bool End();
};

class LinkedResourceMgr : public XResourceMgr
{
	HINSTANCE m_inst;
	LinkedFileList m_openfiles;
	HoeRes::SymbolLink* m_sym;
	const tchar ** m_files;
public:
	LinkedResourceMgr(HINSTANCE inst, 
		HoeRes::SymbolLink* sym,
		const tchar ** files);
	// add linkes x auto linkes
	virtual HoeCore::ReadStream * GetResource(const tchar* name, const tchar** ns = NULL);
	HoeCore::ReadStream * LoadSymbol(HoeRes::SymbolLink& sym);
	LinkedFile& GetFile(int n);
};

#endif

} // namespace HoeRes

#endif // _HOERESOURCE_RESMGR_H_
