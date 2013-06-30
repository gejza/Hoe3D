
#ifndef _HOERESOURCE_RESMGR_H_
#define _HOERESOURCE_RESMGR_H_

namespace HoeRes {

struct SymbolLink;
struct SymbolFile;

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
	const HoeRes::SymbolFile* m_sym;
	HGLOBAL m_g;
	byte* m_ptr;
public:
	LinkedFile(int n, const HoeRes::SymbolFile* sym);
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
};

class LinkedResourceMgr : public XResourceMgr
{
	HINSTANCE m_inst;
	LinkedFileList m_openfiles;
	const HoeRes::SymbolLink* m_sym;
	const HoeRes::SymbolFile* m_files;
public:
	LinkedResourceMgr(HINSTANCE inst, 
		const HoeRes::SymbolLink* sym,
		const HoeRes::SymbolFile* files);
	// add linkes x auto linkes
	virtual HoeCore::ReadStream * GetResource(const tchar* name, const tchar** ns = NULL);
	HoeCore::ReadStream * LoadSymbol(const HoeRes::SymbolLink& sym);
	LinkedFile& GetFile(int n);
};

#endif

} // namespace HoeRes

#endif // _HOERESOURCE_RESMGR_H_
