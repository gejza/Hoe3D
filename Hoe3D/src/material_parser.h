
#ifndef _MATERIAL_PARSER_H_
#define _MATERIAL_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int type;
	union {
	char text[256];
	};
} mtl_value;

int material_load(const char * file);
int material_parse(mtl_value * val);
void material_free();

#define MTLP_ERROR -1
#define MTLP_EQ 1
#define MTLP_TEXT 2

#define MTLP_TEXTURE 10

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _MATERIAL_PARSER_H_

