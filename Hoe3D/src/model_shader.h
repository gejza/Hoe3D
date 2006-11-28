
#ifndef _HOE_MODEL_SHADER_H_
#define _HOE_MODEL_SHADER_H_

enum hmsInstruktion
{
	hmsInfo = 1,
	hmsResources,
	hmsRender,
	hmsEnd = 0x7fffffff
};

// header
#define IDHMSHEADER(a,b)	('M'+('S'<<8)+((a+'0')<<16)+((b+'0')<<24))

// info
struct hms_info
{
	int num_streams;
	int num_indices;
	int num_materials;
};

// resources
#define HMS_STREAM 1
#define HMS_INDEX 2
#define HMS_MATERIAL 3
#define HMS_POINT 4


#define HMS_GENID(type,pos) ((type << 24) | pos)

#endif // _HOE_MODEL_SHADER_H_

