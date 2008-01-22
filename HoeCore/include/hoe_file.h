
#ifndef _HOE_CORE_FILE_H_
#define _HOE_CORE_FILE_H_

#include "hoe_stream.h"
#include "hoe_string.h"
#include "hoe_structures.h"

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

class FileReader;

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
	uint m_refs;
	size_t m_pos;

	struct Buffer
	{
		void * ptr;
		size_t size;
		size_t tell;
		Buffer() : ptr(NULL), size(0), tell(0) {}
		~Buffer() { if (ptr) free(ptr); }
		bool Alloc(size_t s)
		{
			ptr = malloc(size = s);
			return ptr != 0;
		}
	};
	HoeCore::List<Buffer> m_buffs;

public:
	File() : m_file(InvalidHandle), m_refs(0) {}
	virtual ~File();
	bool Open(const CString name, EHoeFileMode mode = hftRead);
	static const char * GetOpenFunc();
	const HoeCore::String& GetName() const { return m_name; }
	bool IsOpen() const { return m_file != InvalidHandle; }
	bool Open();
	virtual void Seek(size_t ptr);
	virtual size_t Tell() const;
	virtual bool Skip(size_t size);
	virtual size_t Read(void* ptr, size_t size);
	virtual size_t Write(const void* ptr, size_t size);
	void Reset();
	uint Close();
	virtual bool CanSeek() { return true; }
	virtual void * CreateBuffer(size_t);
	void Flush() {}
	virtual ReadStream* CreateReader(size_t pos);
};

class FileReader : public ReadStream
{
	size_t m_pos;
	File& m_file;
	bool m_destroyself;
	bool m_open;
public:
	FileReader(File& file, size_t pos, bool destroy = false);
	virtual ~FileReader()
	{
		m_destroyself = false;
		Close();
	}
	virtual uint Close();
	virtual size_t Read(void* ptr, size_t size);
	virtual bool CanSeek() { return true; }
	virtual void Seek(size_t pos) { m_pos = pos; }
	virtual ReadStream* CreateReader(size_t pos);
};


bool SetRootDir(const tchar * dir);
const tchar * GetBaseDir(const tchar * path);

} // namespace HoeCore


#endif // _HOE_CORE_FILE_H_

