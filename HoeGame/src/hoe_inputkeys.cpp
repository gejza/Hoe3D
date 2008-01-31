
#include "../include/HoeGame/hoe_inputkeys.h"

#ifndef NULL
#define NULL 0
#endif

static HKeyDesc g_charmap[] = {
	{ 0, 0, NULL },
	{ 0, 0, "Escape" }, // HK_ESCAPE
	{ '1', '!', "1" }, // HK_1
	{ '2', '@', "2" }, // HK_2
	{ '3', '#', "3" }, // HK_3
	{ '4', '$', "4" }, // HK_4
	{ '5', '%', "5" }, // HK_5
	{ '6', '^', "6" }, // HK_6
	{ '7', '&', "7" }, // HK_7
	{ '8', '*', "8" }, // HK_8
	{ '9', '(', "9" }, // HK_9
	{ '0', ')', "0" }, // HK_0
	{ '-', '_', "-" }, // HK_MINUS
	{ '=', '+', "=" }, // HK_EQUALS
	{ 0, 0, "Backspace" }, // HK_BACK
	{ 0, 0, "Tab" }, // HK_TAB
	{ 'q', 'Q', "Q" }, // HK_Q
	{ 'w', 'W', "W" }, // HK_W
	{ 'e', 'E', "E" }, // HK_E
	{ 'r', 'R', "R" }, // HK_R
	{ 't', 'T', "T" }, // HK_T
	{ 'y', 'Y', "Y" }, // HK_Y
	{ 'u', 'U', "U" }, // HK_U
	{ 'i', 'I', "I" }, // HK_I
	{ 'o', 'O', "O" }, // HK_O
	{ 'p', 'P', "P" }, // HK_P
	{ '[', '{', "[" }, // HK_LBRACKET
	{ ']', '}', "]" }, // HK_RBRACKET
	{ 0, 0, "Enter" }, // HK_RETURN
	{ 0, 0, "Left Control" }, // HK_LCONTROL
	{ 'a', 'A', "A" }, // HK_A
	{ 's', 'S', "S" }, // HK_S
	{ 'd', 'D', "D" }, // HK_D
	{ 'f', 'F', "F" }, // HK_F
	{ 'g', 'G', "G" }, // HK_G
	{ 'h', 'H', "H" }, // HK_H
	{ 'j', 'J', "J" }, // HK_J
	{ 'k', 'K', "K" }, // HK_K
	{ 'l', 'L', "L" }, // HK_L
	{ ';', ':', ";" }, // HK_SEMICOLON
	{ '\'', '"', "'" }, // HK_APOSTROPHE
	{ '`', '~', "`" }, // HK_GRAVE
	{ 0, 0, "Left Shift" }, // HK_LSHIFT
	{ '\\', '|', "\\" }, // HK_BACKSLASH
	{ 'z', 'Z', "Z" }, // HK_Z
	{ 'x', 'X', "X" }, // HK_X
	{ 'c', 'C', "C" }, // HK_C
	{ 'v', 'V', "V" }, // HK_V
	{ 'b', 'B', "B" }, // HK_B
	{ 'n', 'N', "N" }, // HK_N
	{ 'm', 'M', "M" }, // HK_M
	{ ',', ',', "," }, // HK_COMMA
	{ '.', '>', "." }, // HK_PERIOD
	{ '/', '?', "/" }, // HK_SLASH
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ ' ', ' ', "Space" }, // HK_SPACE
	{ 0, 0, NULL },
	{ 0, 0, "F1" }, // HK_F1
	{ 0, 0, "F2" }, // HK_F2
	{ 0, 0, "F3" }, // HK_F3
	{ 0, 0, "F4" }, // HK_F4
	{ 0, 0, "F5" }, // HK_F5
	{ 0, 0, "F6" }, // HK_F6
	{ 0, 0, "F7" }, // HK_F7
	{ 0, 0, "F8" }, // HK_F8
	{ 0, 0, "F9" }, // HK_F9
	{ 0, 0, "F10" }, // HK_F10
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, "F11" }, // HK_F11
	{ 0, 0, "F12" }, // HK_F12
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, "F13" }, // HK_F13
	{ 0, 0, "F14" }, // HK_F14
	{ 0, 0, "F15" }, // HK_F15
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, "Home" }, // HK_HOME
	{ 0, 0, "Up" }, // HK_UP
	{ 0, 0, NULL },
	{ 0, 0, NULL },
	{ 0, 0, "Left" }, // HK_LEFT
	{ 0, 0, NULL },
	{ 0, 0, "Right" }, // HK_RIGHT
	{ 0, 0, NULL },
	{ 0, 0, "End" }, // HK_END
	{ 0, 0, "Down" }, // HK_DOWN

};

HKeyDesc * GetKeyDesc(int hkey)
{
	if (hkey >= (sizeof(g_charmap) / sizeof(HKeyDesc)))
	{
		return &g_charmap[0]; // prvni je prazdnej
	}
	else
		return &g_charmap[hkey];
}



