
#ifndef _HOE_CORE_FILE_H_
#define _HOE_CORE_FILE_H_

#include "hoe_stream.h"
#include "hoe_string.h"

namespace HoeCore {

#ifdef _WIN32
#define WINDOWS_FILE_FUNC
#endif

#ifdef WINDOWS_FILE_FUNC
	typedef HANDLE FileHandle;
	static const FileHandle InvalidHandle = INVALID_HANDLE_VALUE;
#else
	typedef FILE* FileHandle;
	static const FileHandle InvalidHandle = NULL;
#endif

class File : public Stream
{
public:
	enum EHoeFileMode
	{
		hftRead,
		hftWrite,
		hftRewrite,
		hftAppend,
		hftTemp,
	};
protected:
	FileHandle m_file;
	HoeCore::String m_name;
	EHoeFileMode m_mode;

	virtual size_t Read(void* ptr, size_t size);
	virtual size_t Write(const void* ptr, size_t size);
public:
	File() : m_file(InvalidHandle) {}
	virtual ~File();
	bool Open(const CString name, EHoeFileMode mode = hftRead);
	static const char * GetOpenFunc();
	const HoeCore::String& GetName() const { return m_name; }
	bool IsOpen() const { return m_file != InvalidHandle; }
	bool Open();
	void Seek(size_t ptr);
	virtual size_t Tell() const;
	virtual bool Skip(size_t size);
	void Reset();
	void Close();
	virtual bool CanSeek() { return true; }
};


} // namespace HoeCore


#endif // _HOE_CORE_FILE_H_

