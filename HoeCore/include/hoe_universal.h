
#ifndef _HOE_UNIVERSAL_H_
#define _HOE_UNIVERSAL_H_

////////////////////////////////
// universal class value
namespace HoeCore {

class Universal
{
public:
	enum Type
	{
		TypeNone,
		TypeString,
		TypeDecimal,
		TypeUnsigned,
		TypeFloat,
		TypeBool,
		TypeFloatVector,
		TypeDecimalVector,
	};
	Universal();
	virtual ~Universal();

	// common functions
	Type GetType() const { return Type(type & 0x0fffffff); }
	bool IsLocked() const { return (type & 0x80000000) != 0; }
	const char * GetTypeName() const { return GetTypeName(GetType()); }
	static const char * GetTypeName(Type t);
	void Lock(Type t) { ConvertTo(t); type = t | 0x80000000; }
	void Unlock() { type = type & 0x0fffffff; }
	void ConvertTo(Type t);

	// setting functions
	void Set(const char * value);
	void Set(float value);
	void Set(unsigned long value);
	void Set(long value);
	void Set(int value);
	void Set(bool value);
	void Set(const Universal & value);
	void Set(const float * v, size_t size);

	// getting functions
	const char * GetStringValue() const;
	bool GetBool() const;
	long GetDecimal() const;
	unsigned long GetUnsigned() const;
	float GetFloat() const;

	float vec_GetFloat(size_t index) const;

	// operators
	const Universal & operator = (const char * value) { Set(value); return *this; }
	const Universal & operator = (float value) { Set(value); return *this; }
	const Universal & operator = (unsigned long value) { Set(value); return *this; }
	const Universal & operator = (long value) { Set(value); return *this; }
	const Universal & operator = (int value) { Set(value); return *this; }
	const Universal & operator = (bool value) { Set(value); return *this; }
	const Universal & operator = (const Universal & value) { Set(value); return *this; }

    operator const char * () const { return GetStringValue(); }
protected:
	void Reset();
	void Clear();

	union Value
	{
		bool b;
		long l;
		unsigned long ul;
		float f;
		char * str;
		const char * cstr;
		float * vf;

	} value;
	dword type;

	size_t size;
	size_t allocated;
};

} // namespace HoeCore

#endif // _HOE_UNIVERSAL_H_

