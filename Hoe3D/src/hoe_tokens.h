
#ifndef _HOE_TOKENS_H_
#define _HOE_TOKENS_H_

#define HOE_T_GENERATE	0x001
#define HOE_T_MODEL		0x101
#define HOE_T_PICTURE	0x102
#define HOE_T_FONT		0x103
#define HOE_T_SOUND		0x104
#define HOE_T_SOUND3D	0x105
#define HOE_T_SOUNDPLAY	0x106
#define HOE_T_PARTICLE  0x107

#define HOE_T_NAME		0x201
#define HOE_T_STRING	0x202

#define HOE_T_INT		0x303

#define HOE_T_CON		0x401
#define HOE_T_FILE		0x402

#define HOE_T_DUMP		0x501

#define HOE_T_BOX		0x601
#define HOE_T_SPHERE	0x602

#define HOE_T_IS_RESOURCE(token) ((token & 0xf00) == 0x100)
#define HOE_T_IS_STRING(token) ((token & 0xf00) == 0x200)
#define HOE_T_IS_LOGTYPE(token) ((token & 0xf00) == 0x400)


#endif // _HOE_TOKENS_H_
