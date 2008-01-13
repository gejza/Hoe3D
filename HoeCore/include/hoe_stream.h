
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
};

class ReadStream : virtual public BaseStream
{
public:
	virtual size_t Read(void* ptr, size_t size) = 0;
};

class WriteStream : virtual public BaseStream
{
public:
	virtual size_t Write(const void* ptr, size_t size) = 0;
	size_t WriteString(const HoeCore::CString str);
};

class Stream : public ReadStream, public WriteStream
{
};

} // namespace HoeCore

#endif // _HOE_CORE_STREAM_H_

