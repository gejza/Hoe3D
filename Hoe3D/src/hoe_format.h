#ifndef _HOE_FORMAT_H_
#define _HOE_FORMAT_H_

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((unsigned long)(unsigned char)(ch0) | ((unsigned long)(unsigned char)(ch1) << 8) |   \
                ((unsigned long)(unsigned char)(ch2) << 16) | ((unsigned long)(unsigned char)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

enum HOEFORMAT
{
    HOE_UNKNOWN              =  0,

    HOE_R8G8B8               = 20,
    HOE_A8R8G8B8             = 21,
    HOE_X8R8G8B8             = 22,
    HOE_R5G6B5               = 23,
    HOE_X1R5G5B5             = 24,
    HOE_A1R5G5B5             = 25,
    HOE_A4R4G4B4             = 26,
    HOE_R3G3B2               = 27,
    HOE_A8                   = 28,
    HOE_A8R3G3B2             = 29,
    HOE_X4R4G4B4             = 30,
	HOE_L8					 = 50,
	HOE_A8L8				 = 51,
	HOE_R8G8B8A8			 = 52,
	HOE_B8G8R8X8			 = 53,
	HOE_B8G8R8A8			 = 54,
	HOE_B8G8R8				 = 55,
	HOE_L8A8				 = 56,

    HOE_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
    HOE_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
    HOE_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
    HOE_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
    HOE_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),


    HOE_D16_LOCKABLE         = 70,
    HOE_D32                  = 71,
    HOE_D15S1                = 73,
    HOE_D24S8                = 75,
    HOE_D24X8                = 77,
    HOE_D24X4S4              = 79,
    HOE_D16                  = 80,

    HOE_D32F_LOCKABLE        = 82,
    HOE_D24FS8               = 83,

    HOE_L16                  = 81,

    HOE_VERTEXDATA           =100,
    HOE_INDEX16              =101,
    HOE_INDEX32              =102,

	HOE_U8V8				 = 201,

	HOE_JPEG				 = MAKEFOURCC('J', 'P', 'E', 'G'),
	HOE_PNG 				 = MAKEFOURCC('P', 'N', 'G', ' '),

    HOE_FORCE_DWORD          =0x7fffffff
};

/////////////////////////
// Func for operation
struct HOECOLOR
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

#define GET_ALPHA_FLOAT(c) (((c & 0xff000000) >> 24) / 255.f)
#define GET_RED_FLOAT(c) (((c & 0x00ff0000) >> 16) / 255.f)
#define GET_GREEN_FLOAT(c) (((c & 0x0000ff00) >> 8) / 255.f)
#define GET_BLUE_FLOAT(c) (((c & 0x000000ff) >> 0) / 255.f)


#ifdef _HOE_D3D_
D3DFORMAT HoeFormatX(HOEFORMAT);
HOEFORMAT HoeFormatX(D3DFORMAT);
#endif // _HOE_D3D9_

#ifdef _HOE_OPENGL_
GLint HoeFormatX(HOEFORMAT);
HOEFORMAT HoeFormatX(GLint);
#endif // _HOE_OPENGL_

#ifndef HOEFORMAT_ONLY_DEF

const char * HoeFormatString(HOEFORMAT);
int HoeFormatSize(HOEFORMAT);
int HoeFormatSizeAlpha(HOEFORMAT);

/*typedef void (*parse_format_func)(unsigned char *,HOECOLOR *);
parse_format_func HoeFormatGetFunc(HOEFORMAT format,bool set);

void HoeFormatConvert(byte * p, size_t height,  HOEFORMAT format);*/
class HFConvert
{
	size_t m_num;
	HOEFORMAT m_from;
	HOEFORMAT m_to;
    bool m_forward;
	byte * m_origin;
public:
	HFConvert(size_t num, HOEFORMAT from, HOEFORMAT to);
	~HFConvert();
	byte * GetPointer(byte * origin);
	void Make();
};

#endif // HOEFORMAT_ONLY_DEF

#endif // _HOE_FORMAT_H_


