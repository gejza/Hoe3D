
#include "StdAfx.h"
#include "fvf.h"

struct flag
{
	char z;
	int size;
	int nums;
	char base;
};

flag prv[] = 
{
	{'f',4,1,'\0'},
	{'x',4,1,'\0'},
	{'i',4,1,'\0'},
	{'b',1,1,'\0'},
	{'w',2,1,'\0'},
	{'1',4,1,'f'},
	{'2',8,2,'f'},
	{'3',12,3,'f'},
	{'4',16,4,'f'},
	{'c',4,1,'x'},
	{'p',12,3,'3'},
	{'r',16,4,'4'},
	{'n',12,3,'3'},
	{'d',4,1,'c'},
	{'s',4,1,'c'},
	{'t',8,2,'2'}
};

inline flag * get_flag(const char c)
{
	for (int i=0;i < sizeof(prv)/sizeof(flag);i++)
		if (prv[i].z == c)
			return &prv[i];
	return NULL;
}

inline flag * get_base_flag(const char c)
{
	flag * f = get_flag(c);
	while (f)
	{
		if (f->base == '\0')
			return f;
		else
			f = get_flag(f->base);
	}
	return NULL;
}



_FVF::_FVF()
{ 
	fvf = "";
	lock_fvf = false;

}

_FVF::_FVF(const char * _fvf)
{
	fvf = _fvf;
	lock_fvf = true;
}

void _FVF::ResetFVF()
{
	if (!lock_fvf)
		fvf = "";
}

void _FVF::AddType(char t)
{
	if (lock_fvf)
		throw "syntax error";

	fvf += t;
}

int _FVF::GetSize()
{
	const char * p = fvf.c_str();
	int size = 0;
	while (*p)
	{
		flag * f = get_flag(*p);
		assert(f);
		size += f->size;
		p++;
	}
	return size;
}

void _FVF::Lock()
{
	if (lock_fvf)
		throw "syntax error";
	lock_fvf = true;
	if (GetSize() == 0)
		throw "empty fvf";
}

/////////////////////

VERTEX::VERTEX(const _FVF & fvf_)
{
	fvf = fvf_.fvf;
	size = this->GetSize();
	assert(size);
	data = (byte*)malloc(size);
	Reset();
}

VERTEX::~VERTEX()
{
	free(data);
}

void * VERTEX::GetData()
{
	assert(data);
	return data;
}

void VERTEX::Next()
{
	p_n++;
	flag * f = get_flag(*p_f);
	if (p_n < f->nums)
		return;
	p_n = 0;
	p_f++;
}

bool VERTEX::AddFloat(float f)
{
	if (*p_f == '\0')
		throw "moc dat v radce";

	float * pf = reinterpret_cast<float*>(p_d);
	if (get_base_flag(*p_f)->z != 'f' || (p_d + 4 > (data + size)))
		return false;
	*pf = f;
	p_d += 4;
	Next();
	return true;
}

bool VERTEX::AddUnsigned(unsigned long dw)
{
	if (*p_f == '\0')
		throw "moc dat v radce";

	flag * f = get_base_flag(*p_f);
	if ((p_d + f->size) > (data + size))
				return false;

	switch (get_base_flag(*p_f)->z)
	{
	case 'f':
        *reinterpret_cast<float*>(p_d) = (float)dw;
		break;
	case 'x':
        *reinterpret_cast<dword*>(p_d) = dw;
		break;
	case 'w':
        *reinterpret_cast<word*>(p_d) = (word)dw;
		break;
	case 'b':
        *reinterpret_cast<byte*>(p_d) = (byte)dw;
		break;
	default:
		return false;
	}
	p_d += f->size;
	Next();
	return true;
}

bool VERTEX::AddNum(int n)
{

	if (*p_f == '\0')
		throw "moc dat v radce";

	flag * f = get_base_flag(*p_f);
	if ((p_d + f->size) > (data + size))
				return false;

	switch (get_base_flag(*p_f)->z)
	{
	case 'f':
        *reinterpret_cast<float*>(p_d) = (float)n;
		break;
	case 'n':
        *reinterpret_cast<int*>(p_d) = n;
		break;
	default:
		return false;
	}
	p_d += f->size;
	Next();
	return true;
}

void VERTEX::Reset()
{
	memset(data,0,size);
	p_f = fvf.c_str();
	p_n = 0;
	p_d = data;
}

#pragma pack(push, 1)

bool VertexTest()
{
	struct TestVertex
	{
		float pos[4];
		float normal[3];
		dword color;
		word slovo;
		float tex[2];
	} testvertex;
	int i;

	float data[4] = { 1.2f,5.4f,8.1f,-5.f};
	memcpy(testvertex.pos,data,16);
	memcpy(testvertex.normal,data,12);

	testvertex.color = 0xff5611;
	testvertex.slovo= 0x45;
	testvertex.tex[0] = 1.5f;
	float texx[2] = { 1.5f, 2.85f };
	memcpy(testvertex.tex,texx,sizeof(float) * 2);

	_FVF fvf;
	fvf.AddType('4');
	fvf.AddType('n');
	fvf.AddType('c');
	fvf.AddType('w');
	fvf.AddType('2');

    VERTEX vertex(fvf);
	if (vertex.GetSize() != sizeof(TestVertex))
	{
		printf("size error - %d\n",sizeof(unsigned short));
		return false;
	}	

	for (i=0;i < 4;i++)
        vertex.AddFloat(data[i]);
	for (i=0;i < 3;i++)
        vertex.AddFloat(data[i]);
	vertex.AddUnsigned(testvertex.color);
	vertex.AddUnsigned(testvertex.slovo);
	for (i=0;i < 2;i++)
        vertex.AddFloat(texx[i]);

	if (vertex.GetSize() != sizeof(TestVertex))
	{
		printf("size error\n");
		return false;
	}

	if (memcmp(vertex.GetData(),&testvertex,sizeof(TestVertex)) != 0)
	{
		printf("data error\n");
		return false;
	}

	printf("Vertex test OK!\n");
	return true;
}

#pragma pack(pop)


