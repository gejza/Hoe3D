
#include "StdAfx.h"
#include "../include/hoe_fixed.h"

/*
 * Tabulks pro výpoèet goniometrických
 * funkcí pomocí algoritmu CORDIC
 */
const HoeMath::fxtype atans[] = 
{
51471, 30385, 16054, 8149, 4090, 2047, 1023, 511,
255, 127, 63, 31, 15, 7, 3, 1,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 
};

/* Konstanta pro regulaci funkci */
const HoeMath::fixed K_fx = 0.6073;

template<int iter> class CORDIC
{
public:
	static inline void rotate(HoeMath::fixed a, 
					HoeMath::fixed& x, 
					HoeMath::fixed& y)
	{
		const int l = HoeMath::fxbits - iter;
		register HoeMath::fixed xn;
		if (a<0) {                      // úhel je záporný => rotace doleva 
            xn.n=x.n + (y.n>>l);           // místo násobení bitový posuv 
            y.n=y.n - (x.n>>l);
            a.n += atans[l];
        }
        else {       
			// úhel je kladný => rotace doprava 
            xn.n=x.n - (y.n>>l);
            y.n=y.n + (x.n>>l);
            a.n -= atans[l];
        }
        x=xn;
		return CORDIC<iter-1>::rotate(a,x,y);
	}
	static inline const HoeMath::fixed sqrt_step(const HoeMath::fixed xi, const HoeMath::fixed y)
	{
		const HoeMath::fixed io = 0.5f;
		return CORDIC<iter-1>::sqrt_step(io*(xi+y/xi), y);
	}
};

template<> void CORDIC<0>::rotate(HoeMath::fixed a, 
					HoeMath::fixed& x, 
					HoeMath::fixed& y)
{
	/* empty */
}

template<> const HoeMath::fixed CORDIC<0>::sqrt_step(const HoeMath::fixed xi, 
										  const HoeMath::fixed y)
{
	return xi;
}

const HoeMath::fixed sinf(HoeMath::fixed n)
{
    HoeMath::fixed x0=1;
    HoeMath::fixed y0=0;

	CORDIC<HoeMath::fxbits>::rotate(n, x0, y0);
    return (y0 * K_fx);        

}

const HoeMath::fixed cosf(HoeMath::fixed n)
{
    HoeMath::fixed x0=1;
    HoeMath::fixed y0=0;

	CORDIC<HoeMath::fxbits>::rotate(n, x0, y0);
    return (x0 * K_fx);        

}

void sincosf(HoeMath::fixed a, HoeMath::fixed* s, HoeMath::fixed* c)
{
    HoeMath::fixed x0=1;
    HoeMath::fixed y0=0;

	CORDIC<HoeMath::fxbits>::rotate(a, x0, y0);
    *c = (x0 * K_fx);        
    *s = (y0 * K_fx);        

}

const HoeMath::fixed tanf(HoeMath::fixed n)
{
    HoeMath::fixed x0=1;
    HoeMath::fixed y0=0;

	CORDIC<HoeMath::fxbits>::rotate(n, x0, y0);
	if (x0==0) 
	{
		HoeMath::fixed f;
		f.n = HoeMath::fxmax;
		return f;
	}
    return (y0 / x0);        

}

const HoeMath::fixed sqrtf(const HoeMath::fixed n)
{
    HoeMath::fixed x0;
	x0.n = n.n >> 1;
	// vice nez 10 iteraci je pro 32bitu zbytecne
	return CORDIC<10>::sqrt_step(x0, n);
       

}


