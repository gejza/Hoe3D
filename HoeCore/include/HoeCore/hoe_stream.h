
#ifndef _HOE_CORE_STREAM_H_
#define _HOE_CORE_STREAM_H_

#include "hoe_mem.h"
#include "hoe_platform.h"

namespace HoeCore {

class CString;
class Endianness;

class BaseStream
{
public:
	BaseStream();
	virtual ~BaseStream();
	virtual bool CanSeek() { return false; }
	virtual void Seek(size_t ptr) {}
	virtual void Skip(size_t ptr) {}
	virtual uint Close() = 0;
	virtual size_t Tell() const;
	virtual const Endianness& GetDataFormat() const;
};

class ReadStream : virtual public BaseStream
{
public:
	virtual size_t Read(void* ptr, size_t size) = 0;
    template<typename TYPE> TYPE Read()
    {
        TYPE t;
        if (Read(&t, sizeof(t)) != sizeof(t))
            throw; // todo
        return t;
    }
	virtual void Skip(size_t ptr) 
	{
		if (CanSeek()) Seek(ptr+Tell());
		else 
		{
			byte buff[1024];
			while (ptr > 0)
			{
				size_t nr = ptr;
				if (nr > 1024) nr = 1024;
				this->Read(buff, nr);
				ptr -= nr;
			}
		}
	}
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
	virtual void Skip(size_t ptr) 
	{
		if (CanSeek()) Seek(ptr+Tell());
		else CreateSpace(ptr);
	}

	int Print(const tchar* fmt, ...);

    template<typename TYPE> size_t WriteStruct(TYPE &t)
    {
        return this->Write(&t, sizeof(TYPE));
    }
    template<typename TYPE> size_t WriteValue(TYPE t)
    {
        return this->WriteStruct<TYPE>(t);
    }

};

class Stream : public ReadStream, public WriteStream
{
public:
	virtual void Skip(size_t ptr) 
	{
		ReadStream::Skip(ptr);
	}
};

class BufferStream
{
public:
	static const int StaticSize = 1000;
private:
	Buffer_s<StaticSize> m_buff;
	Buffer m_dynbuff;
	bool m_usedynamic;
	ReadStream& m_stream;
	size_t m_first, m_end; // zacatek a konec dat v bufferu
	int m_countdynamic;
	bool m_eos;
public:
	BufferStream(ReadStream& stream);
	byte* GetPtr() 
	{
		if (!GetSize()) return ReadNext(StaticSize);
		return (m_usedynamic ? m_dynbuff.GetPtr():m_buff.Get()) + m_first; 
	}
	size_t GetSize() { return m_end-m_first; }
	void Shift(size_t size);
	byte* ReadNext(size_t size);
};

class TextReadStream
{
	Endianness m_end;
	BufferStream m_stream;
	bool m_eos;
public:
	TextReadStream(ReadStream& stream);
	const char* ReadLine();
	const wchar_t* ReadWLine();
	const Endianness& GetDataFormat() const { return m_end; }

};

} // namespace HoeCore

#endif // _HOE_CORE_STREAM_H_

