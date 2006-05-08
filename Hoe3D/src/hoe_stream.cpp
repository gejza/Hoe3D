
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "hoe3d_math.h"
#include "hoe_stream.h"

#ifdef _HOE_D3D_
#define FVF_XYZ D3DFVF_XYZ
#define FVF_XYZRHW D3DFVF_XYZRHW
#define FVF_NORMAL D3DFVF_NORMAL
#define FVF_DIFFUSE D3DFVF_DIFFUSE
#define FVF_SPECULAR D3DFVF_SPECULAR
#define FVF_TEX1 D3DFVF_TEX1
#endif

#ifdef _HOE_OPENGL_
#define FVF_XYZ			(1 << 0)
#define FVF_XYZRHW		(1 << 1) 
#define FVF_NORMAL		(1 << 2)
#define FVF_DIFFUSE		(1 << 3)
#define FVF_SPECULAR	(1 << 4)
#define FVF_TEX1		(1 << 5)
#endif

HoeStream::HoeStream()
{
	m_size = 0;
	m_fvf[0] = 0;
	m_numvert = 0;
}

bool HoeStream::Create(dword numvert,const char * fvf,dword size)
{
	m_size = size;
	strcpy(m_fvf,fvf);
	m_numvert = numvert;

	m_dwfvf = this->GetFVF(fvf);

#ifdef _HOE_OPENGL_
	m_pVertices = new byte[size];
	// jestli i opengl vytvorit paralelne s tim i vertex buffer object
	if (GetRef()->ext.vb.IsSupported())
		GetRef()->ext.vb.glGenBuffersARB(1, &m_vb);
	else
		m_vb = 0;
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_

	if( FAILED( D3DDevice()->CreateVertexBuffer( size,
         D3DUSAGE_DYNAMIC /* Usage */, m_dwfvf, D3DPOOL_DEFAULT, &m_vb RESERVE_PAR ) ) )
		 return false;
#endif // _HOE_D3D9_

	return true;
}

byte * HoeStream::Lock()
{
#ifdef _HOE_D3D_
	if( FAILED( m_vb->Lock( 0, m_size, (D3DLOCKTYPE)&m_pVertices, 0 ) ) )
		return 0;
#endif
	return m_pVertices;
}

void HoeStream::Unlock()
{
	m_box.Compute((const HoeMath::VECTOR3 *)m_pVertices,m_numvert,m_size/m_numvert);
#ifdef _HOE_D3D_
	m_vb->Unlock();
#endif
#ifdef _HOE_OPENGL_
	if (GetRef()->ext.vb.IsSupported())
	{
		GetRef()->ext.vb.glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vb);
		GetRef()->ext.vb.glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_size, m_pVertices, GL_STATIC_DRAW_ARB);
		delete [] m_pVertices;
		m_pVertices = 0;
	}
#endif
}

void HoeStream::Set(int n)
{
#ifdef _HOE_D3D_
	D3DDevice()->SetFVF(m_dwfvf);
#ifdef _HOE_D3D9_
    D3DDevice()->SetStreamSource( n, m_vb, 0, m_size / m_numvert );
#else
    D3DDevice()->SetStreamSource( n, m_vb, m_size / m_numvert );
#endif // _HOE_D3D_
#endif // _HOE_D3D_
#ifdef _HOE_OPENGL_
	const bool vb = GetRef()->ext.vb.IsSupported();
	int stride = 0;
	if (vb)
		GetRef()->ext.vb.glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_vb);
	if (m_dwfvf & FVF_XYZ)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,m_size / m_numvert,vb ? NULL:m_pVertices);
		stride += 3*sizeof(float);
	}
	else if (m_dwfvf & FVF_XYZRHW)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4,GL_FLOAT,m_size / m_numvert,vb ? NULL:m_pVertices);
		stride += 4*sizeof(float);
	}
	else
		glDisableClientState(GL_VERTEX_ARRAY);
	if (m_dwfvf & FVF_NORMAL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT,m_size / m_numvert,vb ? (byte*)stride:(m_pVertices + stride));
		stride += 3*sizeof(float);
	}
	else
		glDisableClientState(GL_NORMAL_ARRAY);
	if (m_dwfvf & FVF_TEX1)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,m_size / m_numvert,vb ? (byte*)stride:(m_pVertices+stride));
		stride += 2*sizeof(float);
	}
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

#endif
}

void HoeStream::Draw()
{

}

dword HoeStream::GetFVF(const char * f)
{
	dword fvf = 0;
	if (*f == 'p')
	{
		f++;
		fvf |= FVF_XYZ;
	}

	if (*f == 'r')
	{
		f++;
		fvf |= FVF_XYZRHW;
	}

	if (*f == 'n')
	{
		f++;
		fvf |= FVF_NORMAL;
	}

	if (*f == 'd')
	{
		f++;
		fvf |= FVF_DIFFUSE;
	}

	if (*f == 's')
	{
		f++;
		fvf |= FVF_SPECULAR;
	}

	if (*f == '2')
	{
		f++;
		fvf |= FVF_TEX1;
	}

	if (*f == 't')
	{
		f++;
		fvf |= FVF_TEX1;
	}

	if (*f == '\0')
		return fvf;

	assert(false && "spatne fvf");
	return 0;
}


