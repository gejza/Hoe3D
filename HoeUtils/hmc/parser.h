
#ifndef _PARSER_H_
#define _PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define P_NL			1
#define P_ERROR			2
#define P_SEP			3
#define P_STT			4
#define P_END			5
#define P_PIPE			6
#define P_EQ			7

#define V_FLOAT			13
#define V_INT			14
#define V_HEX			15
#define V_NAME			16
#define V_STRING		17
#define V_BOOL			18
#define V_NAME_INDEX	19


#define C_STREAM		100
#define C_INDEX			101
#define C_MODEL			102
#define C_MATERIAL		103

#define E_STREAM		200
#define E_INDEX			201
#define E_MODEL			202
#define E_MATERIAL		203

#define M_DIFFUSE		501
#define M_AMBIENT		502
#define M_SPECULAR		503
#define M_TEXTURE		504
#define M_ALPHATEST		505


#define F_XYZ			1001
#define F_XYZRHW		1002
#define F_NORMAL		1003
#define F_TEX2			1004

#define MS_STREAMS		10001
#define MS_INDICES		10002
#define MS_MATERIALS	10008
#define MS_INPUT		10003
#define MS_FPARAM		10004
#define MS_IPARAM		10005
#define MS_STDRENDER	10006
#define MS_ALLRENDER	10007
#define MS_POINT		10008

#define FNC_AUTOTRACKING 100001

struct p_value
{
	union {
		float f;
		int i;
		unsigned long hex;
		unsigned char byte;
		char * str;
		char c;
	};
};

int lexln(struct p_value * val);
void parse_load(const char *str);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _PARSER_H_
