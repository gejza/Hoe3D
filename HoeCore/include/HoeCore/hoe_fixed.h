
#ifndef _HOE_FIXED_H_
#define _HOE_FIXED_H_

#ifdef __cplusplus

namespace HoeMath {

#ifdef _LINUX
    typedef int64_t int64;
#endif

typedef int fxtype;
const int fxbits = 16;
const int fxmax = 0x7ffffff;
const int fxmin = 0x8000000;
const float fxbase = (float)(1 << fxbits);

struct fixed
{
	fxtype n;

	fixed() {}
	fixed(const int a) 	{		n = a << fxbits;
	}	fixed(const uint a) 	{		n = a << fxbits;
	}	fixed(const float a)
	{
		n = (fxtype)(a * fxbase);
	}
	fixed(const double a)
	{
		n = (fxtype)(a * fxbase);
	}

	fixed& operator = (const float a)
	{
		n = (fxtype)(a * fxbase);
		return *this;
	}
	fixed& operator = (const int a)
	{
		n = a << fxbits;
		return *this;
	}
	const fixed& operator *= (const fixed& a)
	{
		n = ((fxtype)(((int64)(a.n) * (int64)(n)) >> fxbits));
        return *this;
	}
	fixed operator * (const fixed& a) const
	{
        fixed ret;
		ret.n = ((fxtype)(((int64)(a.n) * (int64)(n)) >> fxbits));
        return ret;
	}
	fixed operator * (const float a) const
	{
        fixed ret;
		ret.n = ((fxtype)(((int64)(a * fxbase) * (int64)(n)) >> fxbits));
        return ret;
	}
	fixed operator * (const int a) const
	{
        fixed ret;
		ret.n = ((fxtype)(((int64)(a << fxbits) * (int64)(n)) >> fxbits));
        return ret;
	}
	const fixed& operator /= (const fixed& a)
	{
		n = (fxtype)(((int64)(n) << fxbits) / (int64)(a.n));
        return *this;
	}
	const fixed operator / (const fixed& a) const
	{
        fixed ret;
		ret.n = (fxtype)(((int64)(n) << fxbits) / (int64)(a.n));
        return ret;
	}
	const fixed operator / (const float a) const
	{
        fixed ret;
		ret.n = (fxtype)(((int64)(n) << fxbits) / (int64)(a * fxbase));
        return ret;
	}
	const fixed operator / (const int a) const
	{
        fixed ret;
		ret.n = (fxtype)(((int64)(n)) / (int64)(a));
        return ret;
	}
	const fixed operator / (const uint a) const
	{
        fixed ret;
		ret.n = (fxtype)(((int64)(n)) / (int64)(a));
        return ret;
	}
	const fixed& operator += (const fixed& a)
	{
        n += a.n;
        return *this;
	}
	const fixed operator + (const fixed& a) const
    {
        fixed ret;
        ret.n = n + a.n;
        return ret;
	}
	const fixed operator + (const float a) const
    {
        fixed ret;
        ret.n = n + (fxtype)(a * fxbase);
        return ret;
	}
	const fixed operator + (const int a) const
    {
        fixed ret;
        ret.n = n + (fxtype)(a << fxbits);
        return ret;
	}
	const fixed& operator -= (const fixed& a)
	{
        n -= a.n;
        return *this;
	}
	const fixed operator - (const fixed& a) const
	{
        fixed ret;
        ret.n = n - a.n;
        return ret;
	}
	fixed operator - (const int a) const
	{
        fixed ret;
        ret.n = n - (a << fxbits);
        return ret;
	}
	fixed operator - (const float a) const
	{
        fixed ret;
        ret.n = n - (fxtype)(a * fxbase);
        return ret;
	}
	fixed operator - () const
	{
        fixed ret;
        ret.n = -n;
        return ret;
	}
	operator float () const
	{
		return n  / fxbase;
	}
	operator int () const
	{
		return n >> fxbits;
	}
	operator uint () const
	{
		return n >> fxbits;
	}
	operator long () const
	{
		return n >> fxbits;
	}
	operator unsigned long () const
	{
		return n >> fxbits;
	}
};

inline fixed operator * (const float a, const fixed& n)
{
    fixed ret;
	ret.n = ((fxtype)(((int64)(a * fxbase) * (int64)(n.n)) >> fxbits));
    return ret;
}

inline fixed operator * (const int a, const fixed& n)
{
    fixed ret;
	ret.n = ((fxtype)(((int64)(a << fxbits) * (int64)(n.n)) >> fxbits));
    return ret;
}

inline fixed operator * (const uint a, const fixed& n)
{
    fixed ret;
	ret.n = ((fxtype)(((int64)(a << fxbits) * (int64)(n.n)) >> fxbits));
    return ret;
}

/*inline fixed operator * (const byte a, const fixed& n)
{
    fixed ret;
	ret.n = ((fxtype)(((int64)(a << fxbits) * (int64)(n.n)) >> fxbits));
    return ret;
}*/


/*const fixed operator / (const int a, const fixed& n) const
{
    fixed ret;
	ret.n = (fxtype)(((int64)(n) << (2*fxbits)) / (int64)(n.n));
    return ret;
}*/

inline fixed operator - (const int a, const fixed& n)
{
    fixed ret;
	ret.n = (a << fxbits) - n.n;
    return ret;
}

inline fixed operator - (const float a, const fixed n)
{
    fixed ret;
	ret.n = (fxtype)(a * fxbase) - n.n;
    return ret;
}

inline fixed operator + (const int a, const fixed& n)
{
    fixed ret;
	ret.n = (a << fxbits) + n.n;
    return ret;
}

// compare
inline bool operator == (const fixed& a, const fixed& b)
{
    return a.n == b.n;
}

inline bool operator == (const fixed& a, const int b)
{
    return a.n == b << fxbits;
}

inline bool operator == (const fixed& a, const float b)
{
    return a.n == b * fxbase;
}

inline bool operator <= (const fixed& a, const fixed& b)
{
    return a.n <= b.n;
}

inline bool operator <= (const fixed& a, const int b)
{
	return a.n <= (b << fxbits);
}

inline bool operator <= (const fixed& a, const float b)
{
	return a.n <= (b * fxbase);
}

inline bool operator >= (const fixed& a, const fixed& b)
{
    return a.n >= b.n;
}

inline bool operator > (const fixed& a, const fixed& b)
{
    return a.n > b.n;
}

inline bool operator > (const fixed& a, const float b)
{
    return a.n > (fxtype)(b * fxbase);
}

inline bool operator > (const fixed& a, const int b)
{
    return a.n > (fxtype)(b << fxbits);
}

inline bool operator < (const fixed& a, const fixed& b)
{
    return a.n < b.n;
}

inline bool operator < (const fixed& a, const float b)
{
    return a.n < (fxtype)(b * fxbase);
}

inline bool operator < (const fixed& a, const int b)
{
    return a.n < (fxtype)(b << fxbits);
}



} // namespace HoeMath

// functions?
inline const HoeMath::fixed fabs(const HoeMath::fixed n)
{
	HoeMath::fixed ret;
	ret.n = n.n & HoeMath::fxmax;
	return ret;
}

const HoeMath::fixed sinf(HoeMath::fixed n);
const HoeMath::fixed cosf(HoeMath::fixed n);
void sincosf(HoeMath::fixed a, HoeMath::fixed* s, HoeMath::fixed* c);
const HoeMath::fixed tanf(HoeMath::fixed n);

const HoeMath::fixed sqrtf(const HoeMath::fixed n);

#endif // __cplusplus

#endif // _HOE_FIXED_H_
