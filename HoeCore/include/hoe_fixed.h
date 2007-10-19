
#ifndef _HOE_FIXED_H_
#define _HOE_FIXED_H_

#ifdef __cplusplus

namespace HoeMath {

#ifdef _LINUX
    typedef int64_t _int64;
#endif

typedef int fxtype;
const int fxbits = 16;
const float fxbase = (float)(1 << fxbits);

class fixed
{
	fxtype n;
public:
	/*fixed() {}
	fixed(int a) { n = a; }*/
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
	fixed operator * (const fixed& a) const
	{
        fixed ret;
		ret.n = ((fxtype)(((_int64)(a.n) * (_int64)(n)) >> fxbits));
        return ret;
	}
	fixed operator / (const fixed& a) const
	{
        fixed ret;
		ret.n = (fxtype)(((_int64)(n) << fxbits) / (_int64)(a.n));
        return ret;
		

	}
	fixed operator + (const fixed& a) const
    {
        fixed ret;
        ret.n = n + a.n;
        return ret;
	}
	fixed operator - (const fixed& a) const
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
	operator float () const
	{
		return n  / fxbase;
	}
	operator int () const
	{
		return n >> fxbits;
	}
};

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_FIXED_H_
