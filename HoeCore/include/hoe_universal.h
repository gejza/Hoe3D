
#ifndef _HOE_UNIVERSAL_H_
#define _HOE_UNIVERSAL_H_

////////////////////////////////
// universal class value
namespace HoeCore {

class MemoryPool;

class Universal
{
public:
	// types
	typedef long TDecimal;
	typedef float TReal;

	enum Type
	{
		TypeNone,
		TypeString,
		TypeWString,
		TypeDecimal,
		TypeUnsigned,
		TypeFloat,
		TypeBool,
		TypePointer,
		TypeData,
	};
	Universal();
	Universal(const char *, HoeCore::MemoryPool* pool = NULL);
	Universal(TReal value, Type=TypeFloat);
	Universal(unsigned long value);
	Universal(TDecimal value);
	Universal(int value);
	Universal(bool value);
	Universal(const Universal & value, HoeCore::MemoryPool* pool = NULL);
	Universal(const void * v, size_t size, HoeCore::MemoryPool* pool = NULL);
	Universal(void * p);

	virtual ~Universal();

	// common functions
	Type GetType() const { return Type(type & 0x0fffffff); }
	bool IsLocked() const { return (type & 0x80000000) != 0; }
	bool IsPooled() const { return (type & 0x40000000) != 0; }
	const char * GetTypeName() const { return GetTypeName(GetType()); }
	static const char * GetTypeName(Type t);
	void Lock(Type t) { ConvertTo(t); type = t | 0x80000000; }
	void Unlock() { type = type & 0x7fffffff; }
	void ConvertTo(Type t);

	// setting functions
	void Set(const char * value, HoeCore::MemoryPool* pool = NULL);
	void Set(const wchar_t * value, HoeCore::MemoryPool* pool = NULL);
	void Set(TReal value, Type type=TypeFloat);
	void Set(unsigned long value);
	void Set(TDecimal value);
	void Set(int value);
	void Set(bool value);
	void Set(const Universal & value, HoeCore::MemoryPool* pool = NULL);
	void Set(const void * v, size_t size, HoeCore::MemoryPool* pool = NULL);
	void Set(void * p);

	// getting functions
	const tchar * GetStringValue() const;
	const char * GetUtfStringValue() const;
	const wchar_t * GetWideStringValue() const;
	bool GetBool() const;
	TDecimal GetDecimal() const;
	unsigned long GetUnsigned() const;
	TReal GetFloat() const;
	void * GetPointer() const;
	size_t GetSize() const { return size; }
	template<typename T> T * GetPtr() const
	 { return reinterpret_cast<T*>(GetPointer());}

	//float vec_GetFloat(size_t index) const;

	// operators
	template<typename TYPE> 
    const Universal & operator = (TYPE value) { Set(value); return *this; }

    operator const char * () const { return GetUtfStringValue(); }
    operator const wchar_t * () const { return GetWideStringValue(); }
protected:
	void Reset();
	void Clear();

	union Value
	{
		bool b;
		TDecimal l;
		unsigned long ul;
		TReal f;
		char * str;
		const char * cstr;
		wchar_t * wstr;
		const wchar_t * cwstr;
		void * p;

	} value;
	dword type;

	size_t size;
	size_t allocated;
};

} // namespace HoeCore

#endif // _HOE_UNIVERSAL_H_

