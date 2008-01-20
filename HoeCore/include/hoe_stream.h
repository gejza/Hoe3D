
#ifndef _HOE_CORE_STREAM_H_
#define _HOE_CORE_STREAM_H_

namespace HoeCore {

class CString;

class BaseStream
{
public:
	BaseStream();
	virtual ~BaseStream();
	virtual bool CanSeek() { return false; }
	virtual void Seek(size_t ptr) {}
	virtual uint Close() = 0;
	virtual size_t Tell() const;
};

class ReadStream : virtual public BaseStream
{
public:
	virtual size_t Read(void* ptr, size_t size) = 0;
	virtual ReadStream* CreateReader(size_t pos) { return NULL; }
	ReadStream* CreateReader() { return CreateReader(Tell()); }
};

class WriteStream : virtual public BaseStream
{
public:
	virtual size_t Write(const void* ptr, size_t size) = 0;
	virtual size_t Write(ReadStream& str, size_t size = 0);
	size_t WriteString(const HoeCore::CString str);
	virtual void * CreateBuffer(size_t);
	void CreateSpace(size_t);
};

class Stream : public ReadStream, public WriteStream
{
};

} // namespace HoeCore

#endif // _HOE_CORE_STREAM_H_

