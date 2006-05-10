
#include "StdAfx.h"
#include <hoe3d.h>
#include "../include/hoe_inputkeys.h"
#include "../include/hoe_gui.h"

BEGIN_HOEGAME

TextDevice::TextDevice()
{
	shift = 0;
}

void TextDevice::_KeyDown(int k)
{
	if (!Key(k))
		return;

	switch (k)
	{
	case HK_BACK:
        Back();
		break;
	case HK_LSHIFT:
		shift |= 2;
		break;
	case HK_RSHIFT:
		shift |= 1;
		break;
	default:
		if (shift && GetKeyDesc(k)->upper)
		{
			AddChar(GetKeyDesc(k)->upper);
			return;
		}
		else if (GetKeyDesc(k)->lower)
		{
			AddChar(GetKeyDesc(k)->lower);
			return;
		}
	}
}

void TextDevice::_KeyUp(int k)
{
	switch (k)
	{
	case HK_LSHIFT:
		shift &= ~2;
		break;
	case HK_RSHIFT:
		shift &= ~1;
		break;
	}
}

END_HOEGAME

