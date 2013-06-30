
#ifndef _HOE_CORE_PLATFORM_H_
#define _HOE_CORE_PLATFORM_H_

/* pro big endian */
#if 1 // little endian
#define LOW_ENDIAN 1
#define LOW_IEEE_ENDIAN 1
#endif

//#pragma warning( push )
//#pragma warning( disable: 4293 )

template<typename V, typename S> V safe_lshift(V v, S s)
{
	if (s > 0)
		return v << s;
	if (s < 0)
		return v >> (-s);
	return v;
}
template<typename V, typename S> V safe_rshift(V v, S s)
{
	if (s > 0)
		return v >> s;
	if (s < 0)
		return v << (-s);
	return v;
}

template<typename TYPE, int size>
class SwitchBytes
{
public:
	static inline TYPE Switch(TYPE t)
	{
		int fr = (sizeof(TYPE) - size) << 3; // odkud
		int to = (size-1) << 3; // kam
		const TYPE t1 = ((TYPE(0xff) << fr) & t);
		if (2*size > sizeof(TYPE)+1)
		{
			const TYPE t2 = safe_lshift(t1, (to - fr));
			//return (((TYPE(0xff) << fr) & t) << (to - fr))
			//	| SwitchBytes<TYPE, size-1>::Switch(t);
            return t2 | SwitchBytes<TYPE, size-1>::Switch(t);
		}
		else
		{
			const TYPE t2 = safe_rshift(t1, (fr - to));
            return t2 | SwitchBytes<TYPE, size-1>::Switch(t);
			/*return (((TYPE(0xff) << fr) & t) >> (fr - to))
				| SwitchBytes<TYPE, size-1>::Switch(t);*/
		}
	}
};

//#pragma warning( pop )

// specialization for 0
template<typename TYPE>
class SwitchBytes<TYPE, 0>
{
public:
	static inline TYPE Switch(TYPE t)
	{
		return 0;
	}
};

template<typename T1,typename T2> 
T2 rewrite_cast(T1 t)
{
	void * tt = &t;
	return *reinterpret_cast<T2*>(tt);
}

template<typename TYPE> TYPE le_num(TYPE type)
{
	if (sizeof(TYPE) == 1)
		return type;
#if LOW_ENDIAN
	return type;
#else
	return SwitchBytes::Switch(type);
#endif
}

template<typename TYPE> TYPE be_num(TYPE type)
{
	if (sizeof(TYPE) == 1)
		return type;
#if LOW_ENDIAN
	return SwitchBytes<TYPE,sizeof(TYPE)>::Switch(type);
#else
	return type;
#endif
}

template<> inline float le_num(float type)
{
#if LOW_ENDIAN
	return type;
#else
	return rewrite_cast<uint32, float>(be_num(rewrite_cast<float,uint32>(type)));
#endif
}

template<> inline float be_num(float type)
{
#if LOW_IEEE_ENDIAN
	return rewrite_cast<uint32, float>
		(SwitchBytes<uint32,4>::Switch(
			rewrite_cast<float,uint32>(type)
				));
#else
	return type;
#endif
}

template<> inline double le_num(double type)
{
#if LOW_IEEE_ENDIAN
	return type;
#else
	return rewrite_cast<uint64, double>
		(SwitchBytes<uint64,8>::Switch(
			rewrite_cast<double,uint64>(type)
				));
#endif
}

template<> inline double be_num(double type)
{
#if LOW_ENDIAN
	return rewrite_cast<uint64, double>(be_num(rewrite_cast<double,uint64>(type)));
#else
	return type;
#endif
}

template<typename TYPE> void le_rep(TYPE& type)
{
	type = le_num<TYPE>(type);
}

template<typename TYPE> void be_rep(TYPE& type)
{
	type = be_num<TYPE>(type);
}

#define be_uint16(n) be_num<uint16>(n)
#define be_uint32(n) be_num<uint32>(n)
#define be_uint64(n) be_num<uint64>(n)
#define be_int16(n) be_num<int16>(n)
#define be_int32(n) be_num<int32>(n)
#define be_int64(n) be_num<int64>(n)

#define le_uint16(n) le_num<uint16>(n)
#define le_uint32(n) le_num<uint32>(n)
#define le_uint64(n) le_num<uint64>(n)
#define le_int16(n) le_num<int16>(n)
#define le_int32(n) le_num<int32>(n)
#define le_int64(n) le_num<int64>(n)
#define le_int(n) le_num<int>(n)
#define le_uint(n) le_num<uint>(n)
#define le_float(n) le_num<float>(n)
#define le_byte(n) le_num<byte>(n)

namespace HoeCore {

class Endianness
{
	uint32 m_end;
public:
	enum {
		LowNum = 0x00,
		BigNum = 0x01,
		LowIEEE = 0x00,
		BigIEEE = 0x02,
		Low = 0x00,
		Big = 0x03,
		MaskEnd = 0x0f,
		ByteChar = 0x00,
		WChar = 0x10,
		WWChar = 0x30,
		MaskChar = 0xf0,
	};

	Endianness(uint32 e);
	Endianness(const Endianness& e);
	static uint32 GetPlatform();
	uint32 Get() const;
	template<typename TYPE> TYPE num(TYPE t) const
	{
		if (m_end & BigNum)
			return be_num<TYPE>(t);
		else
			return le_num<TYPE>(t);
	}
	static const tchar * GetPlatformString(uint32 plat);
	const tchar * GetPlatformString();
	/** Funkce rozpozna kodovani textu Utf-8 nebo Utf-16 
	* @return pocet proskenovanych bytu
	*/
	int SetTextType(const byte* str);

	inline bool IsUnicode() const
	{
		return (m_end & WChar) != 0;
	}
};

template<> inline float Endianness::num(float t) const
{
	if (m_end & BigIEEE)
		return be_num<float>(t);
	else
		return le_num<float>(t);
}

template<> inline double Endianness::num(double t) const
{
	if (m_end & BigIEEE)
		return be_num<double>(t);
	else
		return le_num<double>(t);
}


}; // namespace HoeCore

#endif // _HOE_CORE_PLATFORM_H_

