
#ifndef _HOE_INPUT_KEYS_H_
#define _HOE_INPUT_KEYS_H_

/* Keyboard codes */
#define HK_ESCAPE          0x01
#define HK_1               0x02
#define HK_2               0x03
#define HK_3               0x04
#define HK_4               0x05
#define HK_5               0x06
#define HK_6               0x07
#define HK_7               0x08
#define HK_8               0x09
#define HK_9               0x0A
#define HK_0               0x0B
#define HK_MINUS           0x0C    /* - on main keyboard */
#define HK_EQUALS          0x0D
#define HK_BACK            0x0E    /* backspace */
#define HK_TAB             0x0F
#define HK_Q               0x10
#define HK_W               0x11
#define HK_E               0x12
#define HK_R               0x13
#define HK_T               0x14
#define HK_Y               0x15
#define HK_U               0x16
#define HK_I               0x17
#define HK_O               0x18
#define HK_P               0x19
#define HK_LBRACKET        0x1A
#define HK_RBRACKET        0x1B
#define HK_RETURN          0x1C    /* Enter on main keyboard */
#define HK_LCONTROL        0x1D
#define HK_A               0x1E
#define HK_S               0x1F
#define HK_D               0x20
#define HK_F               0x21
#define HK_G               0x22
#define HK_H               0x23
#define HK_J               0x24
#define HK_K               0x25
#define HK_L               0x26
#define HK_SEMICOLON       0x27
#define HK_APOSTROPHE      0x28
#define HK_GRAVE           0x29    /* accent grave */
#define HK_LSHIFT          0x2A
#define HK_BACKSLASH       0x2B
#define HK_Z               0x2C
#define HK_X               0x2D
#define HK_C               0x2E
#define HK_V               0x2F
#define HK_B               0x30
#define HK_N               0x31
#define HK_M               0x32
#define HK_COMMA           0x33
#define HK_PERIOD          0x34    /* . on main keyboard */
#define HK_SLASH           0x35    /* / on main keyboard */
#define HK_RSHIFT          0x36
#define HK_MULTIPLY        0x37    /* * on numeric keypad */
#define HK_LMENU           0x38    /* left Alt */
#define HK_SPACE           0x39
#define HK_CAPITAL         0x3A
#define HK_F1              0x3B
#define HK_F2              0x3C
#define HK_F3              0x3D
#define HK_F4              0x3E
#define HK_F5              0x3F
#define HK_F6              0x40
#define HK_F7              0x41
#define HK_F8              0x42
#define HK_F9              0x43
#define HK_F10             0x44
#define HK_NUMLOCK         0x45
#define HK_SCROLL          0x46    /* Scroll Lock */
#define HK_NUMPAD7         0x47
#define HK_NUMPAD8         0x48
#define HK_NUMPAD9         0x49
#define HK_SUBTRACT        0x4A    /* - on numeric keypad */
#define HK_NUMPAD4         0x4B
#define HK_NUMPAD5         0x4C
#define HK_NUMPAD6         0x4D
#define HK_ADD             0x4E    /* + on numeric keypad */
#define HK_NUMPAD1         0x4F
#define HK_NUMPAD2         0x50
#define HK_NUMPAD3         0x51
#define HK_NUMPAD0         0x52
#define HK_DECIMAL         0x53    /* . on numeric keypad */
#define HK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define HK_F11             0x57
#define HK_F12             0x58
#define HK_F13             0x64    /*                     (NEC PC98) */
#define HK_F14             0x65    /*                     (NEC PC98) */
#define HK_F15             0x66    /*                     (NEC PC98) */
#define HK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define HK_PREVTRACK       0x90    /* Previous Track (HK_CIRCUMFLEX on Japanese keyboard) */
#define HK_AT              0x91    /*                     (NEC PC98) */
#define HK_COLON           0x92    /*                     (NEC PC98) */
#define HK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define HK_STOP            0x95    /*                     (NEC PC98) */
#define HK_AX              0x96    /*                     (Japan AX) */
#define HK_UNLABELED       0x97    /*                        (J3100) */
#define HK_NEXTTRACK       0x99    /* Next Track */
#define HK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define HK_RCONTROL        0x9D
#define HK_MUTE            0xA0    /* Mute */
#define HK_CALCULATOR      0xA1    /* Calculator */
#define HK_PLAYPAUSE       0xA2    /* Play / Pause */
#define HK_MEDIASTOP       0xA4    /* Media Stop */
#define HK_VOLUMEDOWN      0xAE    /* Volume - */
#define HK_VOLUMEUP        0xB0    /* Volume + */
#define HK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define HK_DIVIDE          0xB5    /* / on numeric keypad */
#define HK_SYSRQ           0xB7
#define HK_RMENU           0xB8    /* right Alt */
#define HK_PAUSE           0xC5    /* Pause */
#define HK_HOME            0xC7    /* Home on arrow keypad */
#define HK_UP              0xC8    /* UpArrow on arrow keypad */
#define HK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define HK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define HK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define HK_END             0xCF    /* End on arrow keypad */
#define HK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define HK_NEXT            0xD1    /* PgDn on arrow keypad */
#define HK_INSERT          0xD2    /* Insert on arrow keypad */
#define HK_DELETE          0xD3    /* Delete on arrow keypad */
#define HK_LWIN            0xDB    /* Left Windows key */
#define HK_RWIN            0xDC    /* Right Windows key */

/* Mouse buttons */
#define HMB_LEFT				0
#define HMB_RIGHT				1
#define HMB_MIDDLE				2

#endif // _HOE_INPUT_KEYS_H_


