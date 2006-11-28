
#ifndef _FVF_H_
#define _FVF_H_

typedef unsigned char byte;
typedef unsigned long dword;
typedef unsigned short word;

#define SETFVF_FLOAT	0xf401
#define SETFVF_FLOAT2	0xf402
#define SETFVF_FLOAT3	0xf403
#define SETFVF_FLOAT4	0xf404
#define SETFVF_COLOR	0xf405
#define SETFVF_XYZ		0xf406
#define SETFVF_XYZRHW	0xf407
#define SETFVF_NORMAL	0xf408
#define SETFVF_DIFFUSE	0xf409
#define SETFVF_SPECULAR	0xf410
#define SETFVF_TEXTURE	0xf411


class _FVF
{
protected:
	std::string fvf;
	bool lock_fvf;
public:
	void ResetFVF();

	_FVF();
	_FVF(const _FVF &);
	_FVF(const char * fvf);
	void AddType(char t);
	void Lock(); 
	int GetSize();
	std::string GetString() { return fvf;}

friend class VERTEX;
};

class VERTEX : public _FVF
{
	byte * data;
	const char * p_f;
	int p_n;
	byte * p_d;
	int size;
	void Next();
public:
	//
	VERTEX(const _FVF &);
	~VERTEX();
	void * GetData();
	bool AddFloat(float);
	bool AddUnsigned(unsigned long);
	bool AddNum(int);
	bool IsEmpty() { return data == p_d;};
	bool IsFull() { return *p_f == '\0'; }
	void Reset();


};

bool VertexTest();

#endif // _FVF_H_

