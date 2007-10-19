
#ifndef _HOE_FIXED_H_
#define _HOE_FIXED_H_

#ifdef __cplusplus

namespace HoeMath {

typedef int fxtype;
const int fxbits = 16;
const float fxbase = (float)(1 << fxbits);

class fixed
{
	fxtype n;
public:
	fixed() {}
	fixed(int a) { n = a; }
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
	fixed operator * (const fixed& a)
	{
		return ((int)(((_int64)(a.n) * (_int64)(n)) >> fxbits));
	}
	fixed operator / (const fixed& a)
	{
		return (int)(((_int64)(n) << fxbits) / (_int64)(a.n));
		

	}
	fixed operator + (const fixed& a)
	{
		return fixed(n + a.n);
	}
	fixed operator - (const fixed& a)
	{
		return fixed(n - a.n);
	}
	operator float ()
	{
		return n  / fxbase;
	}
	operator int ()
	{
		return n >> fxbits;
	}
};

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_FIXED_H_
