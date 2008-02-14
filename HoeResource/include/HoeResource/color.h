
#ifndef _HOE_RESOURCE_COLOR_H_
#define _HOE_RESOURCE_COLOR_H_

#include "hoe_res.h"

namespace HoeRes {

class ColorConv
{
    dword (FCALL* m_to)(const HOECOLOR& c);
	void (FCALL* m_from)(HOECOLOR& c, const dword dw);

	HOEFORMAT m_fromformat, m_toformat;
	uint m_fromnum, m_tonum;
	HOECOLOR * m_colors;
	uint m_numcolors;
public:
	ColorConv(HOEFORMAT from, HOEFORMAT to);
	bool Conv(byte* dest, byte* src);
	void SetSrcPalette(HOECOLOR* c, uint num)
	{
		m_colors = c; m_numcolors = num;
	}
	uint NumSrcByte() { return m_fromnum; }
	uint NumDestByte() { return m_tonum; }
};

dword CompileColor(HOEFORMAT fmt, const HOECOLOR& c);

} // namespace HoeRes

#endif // _HOE_RESOURCE_COLOR_H
