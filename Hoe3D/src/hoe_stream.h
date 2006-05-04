
#ifndef _HOE_STREAM_
#define _HOE_STREAM_

class HoeStream
{
#ifdef _HOE_D3D_
	SysVertexBuffer m_vb;
#elif defined( _HOE_OPENGL_)
#else
#error Render device must be defined.
#endif
	char m_fvf[8];
	dword m_dwfvf;
	dword m_size;
	dword m_numvert;
	byte * m_pVertices;
	HoeMath::BoundingBox m_box;

	dword GetFVF(const char *);
public:
	HoeStream();
	bool Create(dword numvert,const char * fvf,dword size);
	byte * Lock();
	void Unlock();
	/////////////////////
	void Set(int n);
	//dword GetFVF() { return m_fvf;}
	dword GetNumVert() { 
		return m_numvert;
	}
	void Draw();
	const HoeMath::BoundingBox * GetBounding() { return &m_box;}
};

#endif // _HOE_STREAM_
