
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include <hoe_math.h>
#include "hoe_stream.h"

HoeStream::HoeStream(bool dynamic, bool soft)
{
	m_size = 0;
	m_numvert = 0;
	m_dynamic = dynamic; /*!!!*/ // musi byt podporavanej hw (v pripade diRECTx)
	m_pVertices = NULL;
	m_soft = soft;
	m_vb = 0;
}

bool HoeStream::Create(dword numvert,const char * fvf,dword size)
{
	assert(numvert > 0 && fvf && size > 0);
	if (m_numvert == numvert && m_size == size && m_fvf == fvf)
		return true;
	m_size = size;
	m_fvf.Set(fvf);
	m_numvert = numvert;

	SAFE_DELETE_ARRAY(m_pVertices);
	if (!m_soft)
	{
#ifdef _HOE_OPENGL_
	// jestli i opengl vytvorit paralelne s tim i vertex buffer object
	if (!m_dynamic && GetRef()->ext.ARB_vertex_buffer_object)
	{	
		if (!m_vb)
			glGenBuffersARB(1, &m_vb);
	}
	else
		m_vb = 0;
#endif // _HOE_OPENGL_
#ifdef _HOE_D3DA_
	SAFE_RELEASE(m_vb);
	HRESULT hRes = D3DDevice()->CreateVertexBuffer( size,
		m_dynamic ? D3DUSAGE_DYNAMIC:0 /* Usage */, m_fvf.GetFVF(), D3DPOOL_DEFAULT, &m_vb RESERVE_PAR );
	checkres(hRes, "CreateVertexBuffer");
#endif // _HOE_D3D9_
	}

	return true;
}

bool HoeStream::Create(dword numvert,const char * fvf,dword size, byte * data)
{
	if (!Create(numvert, fvf, size))
		return false;
	byte * lc = Lock();
	assert(lc);
	memcpy(lc, data, size);
	Unlock();
	return true;
}

byte * HoeStream::Lock()
{
	#ifdef _HOE_D3DA_
		if (!m_soft)
		{
			if( FAILED( m_vb->Lock( 0, m_size, (D3DLOCKTYPE)&m_pVertices, 0 ) ) )
				return 0;
		}
	#endif
	if (m_pVertices == NULL)
		m_pVertices = new byte[m_size];
	return m_pVertices;
}

void HoeStream::Unlock()
{
	if (!m_dynamic)
		m_box.Compute((const HoeMath::Vector3 *)m_pVertices,m_numvert,m_size/m_numvert);
	if (m_soft)
		return;
#ifdef _HOE_D3DA_
	m_vb->Unlock();
	m_pVertices = NULL;
#endif
#ifdef _HOE_OPENGL_
	if (m_vb != 0)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vb);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_size, m_pVertices, GL_STATIC_DRAW_ARB);
		SAFE_DELETE_ARRAY(m_pVertices);
	}
#endif
}

void HoeStream::Set(int n)
{
	assert(!m_soft && "Soft stream not use for rendering.");
#ifdef _HOE_D3D_
	D3DDevice()->SetFVF(m_fvf.GetFVF());
#ifdef _HOE_D3D9_
    D3DDevice()->SetStreamSource( n, m_vb, 0, m_size / m_numvert );
#else
    D3DDevice()->SetStreamSource( n, m_vb, m_size / m_numvert );
#endif // _HOE_D3D_
#endif // _HOE_D3D_
#ifdef _HOE_OPENGL_
	int stride = 0;
	if (m_vb)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_vb);
		checkgl("glBindBufferARB");
	}
	else
	{
        if (glBindBufferARB)
        {
		    glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
		    checkgl("glBindBufferARB");
        }
	}
	if (m_fvf.GetFVF() & FVF_XYZ)
	{
		glVertexPointer(3,GL_FLOAT,m_size / m_numvert,m_vb ? NULL:m_pVertices);
		checkgl("glVertexPointer");
		glEnableClientState(GL_VERTEX_ARRAY);
		checkgl("glEnableClientState");
		stride += 3*sizeof(float);
	}
	else if (m_fvf.GetFVF() & FVF_XYZRHW)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		checkgl("glEnableClientState");
		glVertexPointer(4,GL_FLOAT,m_size / m_numvert,m_vb ? NULL:m_pVertices);
		checkgl("glVertexPointer");
		stride += 4*sizeof(float);
	}
	else
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		checkgl("glDisableClientState");
	}

	if (m_fvf.GetFVF() & FVF_NORMAL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		checkgl("glEnableClientState");
		glNormalPointer(GL_FLOAT,m_size / m_numvert,m_vb ? (byte*)stride:(m_pVertices + stride));
		checkgl("glNormalPointer");
		stride += 3*sizeof(float);
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
		checkgl("glDisableClientState");
	}
	// diffuse
	if (m_fvf.GetFVF() & FVF_DIFFUSE)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		checkgl("glEnableClientState");
		glColorPointer(4,GL_BYTE,m_size / m_numvert,m_vb ? (byte*)stride:(m_pVertices+stride));
		checkgl("glTexCoordPointer");
		stride += sizeof(dword);
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
		checkgl("glDisableClientState");
	}

	if (glClientActiveTextureARB) {
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	if (m_fvf.GetFVF() & FVF_TEX1)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		checkgl("glEnableClientState");
		glTexCoordPointer(2,GL_FLOAT,m_size / m_numvert,m_vb ? (byte*)stride:(m_pVertices+stride));
		checkgl("glTexCoordPointer");
		stride += 2*sizeof(float);
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		checkgl("glDisableClientState");
	}

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	if (m_fvf.GetFVF() & FVF_TEX2)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		checkgl("glEnableClientState");
		glTexCoordPointer(2,GL_FLOAT,m_size / m_numvert,m_vb ? (byte*)stride:(m_pVertices+stride));
		checkgl("glTexCoordPointer");
		stride += 2*sizeof(float);
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		checkgl("glDisableClientState");
	}

	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	}
#endif
}

void HoeStream::Dump(HoeLog *log)
{
	if (!log)
		return;

	byte * p = this->m_pVertices;
	if (!p)
	{
		log->Log("NULL pointer -> stop dumping");
		return;
	}
	log->Log("Memory address %p -> %p", m_pVertices, m_pVertices+m_size);
	m_box.Compute((const HoeMath::Vector3 *)m_pVertices,m_numvert,m_size/m_numvert);

	size_t stride = m_size / m_numvert;

	for (dword i=0; i < this->m_numvert;i++)
	{
		char line[2000] = {0};
		char tmp[1000] = {0};
		const char * d = this->m_fvf.GetStringFVF();
		p = m_pVertices + (i*stride);
		while (*d)
		{
			switch (*d)
			{
			case 'p':
				{
					float * n = (float*)p;
					sprintf(tmp, "p:(%f, %f, %f) ", n[0], n[1], n[2]);
					strcat(line, tmp);
					p += 12;
					break;
				}
				break;
			case 'n':
				{
					float * n = (float*)p;
					sprintf(tmp, "n:(%f, %f, %f) = %f ", n[0], n[1], n[2], sqrtf(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]));
					strcat(line, tmp);
					p += 12;
					break;
				}
				break;
			//case 'd':

			case 't':
				{
					float * n = (float*)p;
					sprintf(tmp, "t:(%f, %f) ", n[0], n[1]);
					strcat(line, tmp);
					p += 8;
					break;
				}
				break;
			default:
				while (d[1]) d++;
			};
			d++;
		}
		log->Log("%s",line);
	}
	log->Log("box:(%f,%f,%f)-(%f,%f,%f) ball: %f", m_box.min.x, m_box.min.y, m_box.min.z, m_box.max.x,
		m_box.max.y, m_box.max.z, m_box.ball);
}


