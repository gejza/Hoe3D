
#ifndef _HOE_CORE_PLATFORM_H_
#define _HOE_CORE_PLATFORM_H_

/* pro big endian */
#if 1 // little endian
#define LOW_ENDIAN 1
#endif

#pragma warning( push )
#pragma warning( disable: 4293 )


template<typename TYPE, int size>
class SwitchBytes
{
public:
	static inline TYPE Switch(TYPE t)
	{
		const unsigned int fr = (sizeof(TYPE) - size) << 3; // odkud
		const unsigned int to = (size-1) << 3; // kam
		const TYPE t1 = ((TYPE(0xff) << fr) & t);
		if (to > fr)
		{
			const TYPE t2 = t1 << (to - fr);
			//return (((TYPE(0xff) << fr) & t) << (to - fr))
			//	| SwitchBytes<TYPE, size-1>::Switch(t);
            return t2 | SwitchBytes<TYPE, size-1>::Switch(t);
		}
		else if (fr > to)
		{
			const TYPE t2 = t1 >> (fr - to);
            return t2 | SwitchBytes<TYPE, size-1>::Switch(t);
			/*return (((TYPE(0xff) << fr) & t) >> (fr - to))
				| SwitchBytes<TYPE, size-1>::Switch(t);*/
		}
        else
            return 0;
	}
};

#pragma warning( pop )

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
#if LOW_ENDIAN
	return rewrite_cast<uint32, float>(be_num(rewrite_cast<float,uint32>(type)));
#else
	return type;
#endif
}

template<> inline double le_num(double type)
{
#if LOW_ENDIAN
	return type;
#else
	return rewrite_cast<uint64, double>(be_num(rewrite_cast<double,uint64>(type)));
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


#endif // _HOE_CORE_PLATFORM_H_

