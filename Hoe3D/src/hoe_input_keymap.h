
#ifndef HOE_KEYEQ_DEF
#define HOE_KEYEQ_DEF
#endif
/*
 * pouziti :
 * 
 * #define HOE_KEY_DEFINE(a,b,c) case a: return b;
 * switch (key)
 * {
 * #include <tenhle fajl>
 * };
 * 
 * const char * GetKeyName(int key) {
 * #undef HOE_KEY_DEFINE
 * #define HOE_KEY_DEFINE(a,b,c) case a: return c;
 * switch (key)
 * {
 * #include <tenhle fajl>
 * };
 *
 */
// HOE_KEY_DEFINE(hkey,vkey, small, upper, text)
//               Hoe key, virtual key,  small, upper, text
HOE_KEY_DEFINE( HK_ESCAPE, VK_ESCAPE, 0, 0, "Escape")        
HOE_KEY_DEFINE( HK_1, '1', '1', '!', "1")
HOE_KEY_DEFINE( HK_2, '2', '2', '@', "2")
HOE_KEY_DEFINE( HK_3, '3', '3', '#', "3")
HOE_KEY_DEFINE( HK_4, '4', '4', '$', "4")
HOE_KEY_DEFINE( HK_5, '5', '5', '%', "5")
HOE_KEY_DEFINE( HK_6, '6', '6', '^', "6")
HOE_KEY_DEFINE( HK_7, '7', '7', '&', "7")
HOE_KEY_DEFINE( HK_8, '8', '8', '*', "8")
HOE_KEY_DEFINE( HK_9, '9', '9', '(', "9")
HOE_KEY_DEFINE( HK_0, '0', '0', ')', "0")
HOE_KEY_DEFINE( HK_MINUS, VK_OEM_MINUS, '-', '_', "-")           /* - on main keyboard */
HOE_KEY_DEFINE( HK_EQUALS, VK_OEM_PLUS, '=', '+', "=")
HOE_KEY_DEFINE( HK_BACK, VK_BACK, 0, 0, "Backspace")    /* backspace */
HOE_KEY_DEFINE( HK_TAB, VK_TAB, 0, 0, "Tab")
HOE_KEY_DEFINE( HK_Q, 'Q', 'q', 'Q', "Q")
HOE_KEY_DEFINE( HK_W, 'W', 'w', 'W', "W")
HOE_KEY_DEFINE( HK_E, 'E', 'e', 'E', "E")
HOE_KEY_DEFINE( HK_R, 'R', 'r', 'R', "R")
HOE_KEY_DEFINE( HK_T, 'T', 't', 'T', "T")
HOE_KEY_DEFINE( HK_Y, 'Y', 'y', 'Y', "Y")
HOE_KEY_DEFINE( HK_U, 'U', 'u', 'U', "U")
HOE_KEY_DEFINE( HK_I, 'I', 'i', 'I', "I")
HOE_KEY_DEFINE( HK_O, 'O', 'o', 'O', "O")
HOE_KEY_DEFINE( HK_P, 'P', 'p', 'P', "P")
HOE_KEY_DEFINE( HK_LBRACKET, VK_OEM_4, '[', '{', "[")
HOE_KEY_DEFINE( HK_RBRACKET, VK_OEM_6, ']', '}', "]")
HOE_KEY_DEFINE( HK_RETURN, VK_RETURN, 0, 0, "Enter")    /* Enter on main keyboard */
HOE_KEY_DEFINE( HK_LCONTROL, VK_CONTROL, 0, 0, "Left Control")
HOE_KEY_DEFINE( HK_A, 'A', 'a', 'A', "A")
HOE_KEY_DEFINE( HK_S, 'S', 's', 'S', "S")
HOE_KEY_DEFINE( HK_D, 'D', 'd', 'D', "D")
HOE_KEY_DEFINE( HK_F, 'F', 'f', 'F', "F")
HOE_KEY_DEFINE( HK_G, 'G', 'g', 'G', "G")
HOE_KEY_DEFINE( HK_H, 'H', 'h', 'H', "H")
HOE_KEY_DEFINE( HK_J, 'J', 'j', 'J', "J")
HOE_KEY_DEFINE( HK_K, 'K', 'k', 'K', "K")
HOE_KEY_DEFINE( HK_L, 'L', 'l', 'L', "L")
HOE_KEY_DEFINE( HK_SEMICOLON, VK_OEM_1, ';', ':', ";")
HOE_KEY_DEFINE( HK_APOSTROPHE, VK_OEM_7, '\'', '"', "'")
HOE_KEY_DEFINE( HK_GRAVE, VK_OEM_3, '`', '~', "`")    /* accent grave */
HOE_KEY_DEFINE( HK_LSHIFT, VK_SHIFT, 0, 0, "Left Shift")
HOE_KEY_DEFINE( HK_BACKSLASH, VK_OEM_5, '\\', '|', "\\")
HOE_KEY_DEFINE( HK_Z, 'Z', 'z', 'Z', "Z")
HOE_KEY_DEFINE( HK_X, 'X', 'x', 'X', "X")
HOE_KEY_DEFINE( HK_C, 'C', 'c', 'C', "C")
HOE_KEY_DEFINE( HK_V, 'V', 'v', 'V', "V")
HOE_KEY_DEFINE( HK_B, 'B', 'b', 'B', "B")
HOE_KEY_DEFINE( HK_N, 'N', 'n', 'N', "N")
HOE_KEY_DEFINE( HK_M, 'M', 'm', 'M', "M")
HOE_KEY_DEFINE( HK_COMMA, VK_OEM_COMMA, ',', '<', ",")
HOE_KEY_DEFINE( HK_PERIOD, VK_OEM_PERIOD, '.', '>', ".")    /* . on main keyboard */
HOE_KEY_DEFINE( HK_SLASH, VK_OEM_2, '/', '?', "/")    /* / on main keyboard */
HOE_KEY_DEFINE( HK_RSHIFT, VK_RSHIFT, 0, 0, "Right Shift")
HOE_KEY_DEFINE( HK_MULTIPLY, VK_MULTIPLY, '*', '*', "Multiply")    /* * on numeric keypad */
//HOE_KEY_DEFINE( HK_LMENU           0x38    /* left Alt */
HOE_KEY_DEFINE( HK_SPACE, VK_SPACE, ' ', ' ', "Space")
//HOE_KEY_DEFINE( HK_CAPITAL         0x3A
HOE_KEY_DEFINE( HK_F1, VK_F1, 0, 0, "F1")
HOE_KEY_DEFINE( HK_F2, VK_F2, 0, 0, "F2")
HOE_KEY_DEFINE( HK_F3, VK_F3, 0, 0, "F3")
HOE_KEY_DEFINE( HK_F4, VK_F4, 0, 0, "F4")
HOE_KEY_DEFINE( HK_F5, VK_F5, 0, 0, "F5")
HOE_KEY_DEFINE( HK_F6, VK_F6, 0, 0, "F6")
HOE_KEY_DEFINE( HK_F7, VK_F7, 0, 0, "F7")
HOE_KEY_DEFINE( HK_F8, VK_F8, 0, 0, "F8")
HOE_KEY_DEFINE( HK_F9, VK_F9, 0, 0, "F9")
HOE_KEY_DEFINE( HK_F10, VK_F10, 0, 0, "F10")
//HOE_KEY_DEFINE( HK_NUMLOCK         0x45
//HOE_KEY_DEFINE( HK_SCROLL          0x46    /* Scroll Lock */
HOE_KEY_DEFINE( HK_NUMPAD7, VK_NUMPAD7, '7', '7', "Num 7")
HOE_KEY_DEFINE( HK_NUMPAD8, VK_NUMPAD8, '8', '8', "Num 8")
HOE_KEY_DEFINE( HK_NUMPAD9, VK_NUMPAD9, '9', '9', "Num 9")
HOE_KEY_DEFINE( HK_SUBTRACT, VK_SUBTRACT, '-', '-', "Substract")   /* - on numeric keypad */
HOE_KEY_DEFINE( HK_NUMPAD4, VK_NUMPAD4, '4', '4', "Num 4")
HOE_KEY_DEFINE( HK_NUMPAD5, VK_NUMPAD5, '5', '5', "Num 5")
HOE_KEY_DEFINE( HK_NUMPAD6, VK_NUMPAD6, '6', '6', "Num 6")
HOE_KEY_DEFINE( HK_ADD, VK_ADD, '+', '+', "Add")    /* + on numeric keypad */
HOE_KEY_DEFINE( HK_NUMPAD1, VK_NUMPAD1, '1', '1', "Num 1")
HOE_KEY_DEFINE( HK_NUMPAD2, VK_NUMPAD2, '2', '2', "Num 2")
HOE_KEY_DEFINE( HK_NUMPAD3, VK_NUMPAD3, '3', '3', "Num 3")
HOE_KEY_DEFINE( HK_NUMPAD0, VK_NUMPAD0, '0', '0', "Num 0")
HOE_KEY_DEFINE( HK_DECIMAL, VK_DECIMAL, '.', '.', "Decimal")    /* . on numeric keypad */
/*HOE_KEY_DEFINE( HK_OEM_102, VK_OEM_102, 0, 0, "OEM 102")    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
HOE_KEY_DEFINE( HK_F11, VK_F11, 0, 0, "F11")
HOE_KEY_DEFINE( HK_F12, VK_F12, 0, 0, "F12")
HOE_KEY_DEFINE( HK_F13, VK_F13, 0, 0, "F13")    /*                     (NEC PC98) */
HOE_KEY_DEFINE( HK_F14, VK_F14, 0, 0, "F14")    /*                     (NEC PC98) */
HOE_KEY_DEFINE( HK_F15, VK_F15, 0, 0, "F15")    /*                     (NEC PC98) */
//HOE_KEY_DEFINE( HK_KANA            0x70    /* (Japanese keyboard)            */
//HOE_KEY_DEFINE( HK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
//HOE_KEY_DEFINE( HK_CONVERT         0x79    /* (Japanese keyboard)            */
//HOE_KEY_DEFINE( HK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
//HOE_KEY_DEFINE( HK_YEN             0x7D    /* (Japanese keyboard)            */
//HOE_KEY_DEFINE( HK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
//HOE_KEY_DEFINE( HK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
//HOE_KEY_DEFINE( HK_PREVTRACK       0x90    /* Previous Track (HK_CIRCUMFLEX on Japanese keyboard) */
//HOE_KEY_DEFINE( HK_AT              0x91    /*                     (NEC PC98) */
//HOE_KEY_DEFINE( HK_COLON           0x92    /*                     (NEC PC98) */
//HOE_KEY_DEFINE( HK_UNDERLINE       0x93    /*                     (NEC PC98) */
//HOE_KEY_DEFINE( HK_KANJI           0x94    /* (Japanese keyboard)            */
//HOE_KEY_DEFINE( HK_STOP            0x95    /*                     (NEC PC98) */
//HOE_KEY_DEFINE( HK_AX              0x96    /*                     (Japan AX) */
//HOE_KEY_DEFINE( HK_UNLABELED       0x97    /*                        (J3100) */
//HOE_KEY_DEFINE( HK_NEXTTRACK       0x99    /* Next Track */
//HOE_KEY_DEFINE( HK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
//HOE_KEY_DEFINE( HK_RCONTROL        0x9D
//HOE_KEY_DEFINE( HK_MUTE            0xA0    /* Mute */
//HOE_KEY_DEFINE( HK_CALCULATOR      0xA1    /* Calculator */
//HOE_KEY_DEFINE( HK_PLAYPAUSE       0xA2    /* Play / Pause */
//HOE_KEY_DEFINE( HK_MEDIASTOP       0xA4    /* Media Stop */
//HOE_KEY_DEFINE( HK_VOLUMEDOWN      0xAE    /* Volume - */
//HOE_KEY_DEFINE( HK_VOLUMEUP        0xB0    /* Volume + */
//HOE_KEY_DEFINE( HK_WEBHOME         0xB2    /* Web home */
//HOE_KEY_DEFINE( HK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
HOE_KEY_DEFINE( HK_DIVIDE, VK_DIVIDE, '/', '/', "Divide")    /* / on numeric keypad */
//HOE_KEY_DEFINE( HK_SYSRQ           0xB7
//HOE_KEY_DEFINE( HK_RMENU           0xB8    /* right Alt */
HOE_KEY_DEFINE( HK_PAUSE, VK_PAUSE, 0, 0, "Pause")    /* Pause */
HOE_KEY_DEFINE( HK_HOME, VK_HOME, 0, 0, "Home")    /* Home on arrow keypad */
HOE_KEY_DEFINE( HK_UP, VK_UP, 0, 0, "Up")    /* UpArrow on arrow keypad */
HOE_KEY_DEFINE( HK_PRIOR, VK_PRIOR, 0, 0, "PgUp")     /* PgUp on arrow keypad */
HOE_KEY_DEFINE( HK_LEFT, VK_LEFT, 0, 0, "Left")
HOE_KEY_DEFINE( HK_RIGHT, VK_RIGHT, 0, 0, "Right")
HOE_KEY_DEFINE( HK_END, VK_END, 0, 0, "End")    /* End on arrow keypad */
HOE_KEY_DEFINE( HK_DOWN, VK_DOWN, 0, 0, "Down")    /* DownArrow on arrow keypad */
HOE_KEY_DEFINE( HK_NEXT, VK_NEXT, 0, 0, "PgDn")    /* PgDn on arrow keypad */
HOE_KEY_DEFINE( HK_INSERT, VK_INSERT, 0, 0, "Insert")    /* Insert on arrow keypad */
HOE_KEY_DEFINE( HK_DELETE, VK_DELETE, 0, 0, "Delete")    /* Delete on arrow keypad */
//HOE_KEY_DEFINE( HK_LWIN            0xDB    /* Left Windows key */
//HOE_KEY_DEFINE( HK_RWIN            0xDC    /* Right Windows key */
//HOE_KEY_DEFINE( HK_APPS            0xDD    /* AppMenu key */
//HOE_KEY_DEFINE( HK_POWER           0xDE    /* System Power */
//HOE_KEY_DEFINE( HK_SLEEP           0xDF    /* System Sleep */
//HOE_KEY_DEFINE( HK_WAKE            0xE3    /* System Wake */
//HOE_KEY_DEFINE( HK_WEBSEARCH       0xE5    /* Web Search */
//HOE_KEY_DEFINE( HK_WEBFAVORITES    0xE6    /* Web Favorites */
//HOE_KEY_DEFINE( HK_WEBREFRESH      0xE7    /* Web Refresh */
//HOE_KEY_DEFINE( HK_WEBSTOP         0xE8    /* Web Stop */
//HOE_KEY_DEFINE( HK_WEBFORWARD      0xE9    /* Web Forward */
//HOE_KEY_DEFINE( HK_WEBBACK         0xEA    /* Web Back */
//HOE_KEY_DEFINE( HK_MYCOMPUTER      0xEB    /* My Computer */
//HOE_KEY_DEFINE( HK_MAIL            0xEC    /* Mail */
//HOE_KEY_DEFINE( HK_MEDIASELECT     0xED    /* Media Select */

