
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "hoe3d_math.h"
#include "config.h"
#include "camera.h"
#include "hoe_time.h"

using namespace HoeMath;

uint HoeCamera::m_width = 0;
uint HoeCamera::m_height = 0;

HoeCamera::HoeCamera()
{
	//pos.part = NULL;
	pos.xyz = VECTOR3(0,0,-100);
	look = VECTOR3(0,0,1);
	matProj.PerspectiveFov(HOE_PIF/4.0f, 1.0f, 1.f, 10000.0f );

	//SetPerspective(GetConfig()->GetCameraFov(),GetConfig()->GetCameraZn(),GetConfig()->GetCameraZf());

	Update();

	Con_Print("camera created");
}

void HoeCamera::SetView(uint w,uint h)
{
    if (h == 0)    /* Prevent A Divide By Zero If The Window Is Too Small */
        h = 1;

	m_width = w;
	m_height = h;
#ifdef _HOE_D3D_
	D3DViewport vp;
	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = w;
	vp.Height = h;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	if (D3DDevice())
		D3DDevice()->SetViewport(&vp);
#endif
#ifdef _HOE_OPENGL_
 	glViewport(0, 0, w, h);    /* Reset The Current Viewport And Perspective Transformation */
#endif
}

void HoeCamera::SetupMatrices()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetTransform( D3DTS_VIEW, matView );
	D3DDevice()->SetTransform( D3DTS_PROJECTION, matProj );
#endif
#ifdef _HOE_OPENGL_
	glMatrixMode(GL_PROJECTION);// Zvolí projekèní matici
	glLoadMatrixf(matViewProj);
	
	glMatrixMode(GL_MODELVIEW);// Zvolí matici modelview
	glLoadIdentity();

#endif

}

void HoeCamera::Setup2DMatrices(const float w,const float h)
{
	MATRIX matWorld;
	matWorld.Ortho( 0,(w > 0) ? w:m_width,(h > 0) ? h:m_height,0,-1,1); 

#ifdef _HOE_D3D_
	MATRIX matView;

	matView.Identity();

	D3DDevice()->SetTransform( D3DTS_WORLD, matWorld );
	D3DDevice()->SetTransform( D3DTS_VIEW, matView );
	D3DDevice()->SetTransform( D3DTS_PROJECTION, matView );
#endif
#ifdef _HOE_OPENGL_
	glMatrixMode(GL_PROJECTION);// Zvolí projekèní matici
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);// Zvolí matici modelview
	glLoadMatrixf((GLfloat*)&matWorld);
#endif
}

void HoeCamera::Set(float * p,float * l)
{
	//this->pos.part = (HoeMapPart *)part;
	this->pos.xyz.Set(p);
	look.Set(l);
	Update();
}

void HoeCamera::CalculateFrustum()
{
	m_frustrum[RIGHT].Set(matViewProj._14 - matViewProj._11,matViewProj._24 - matViewProj._21,matViewProj._34 - matViewProj._31,matViewProj._44 - matViewProj._41);
	m_frustrum[RIGHT].Normalize();
	
	m_frustrum[LEFT].Set(matViewProj._14 + matViewProj._11,matViewProj._24 + matViewProj._21,matViewProj._34 + matViewProj._31,matViewProj._44 + matViewProj._41);
	m_frustrum[LEFT].Normalize();
	
	m_frustrum[BOTTOM].Set(matViewProj._14 + matViewProj._12,matViewProj._24 + matViewProj._22,matViewProj._34 + matViewProj._32,matViewProj._44 + matViewProj._42);
	m_frustrum[BOTTOM].Normalize();
	
	m_frustrum[TOP].Set(matViewProj._14 - matViewProj._12,matViewProj._24 - matViewProj._22,matViewProj._34 - matViewProj._32,matViewProj._44 - matViewProj._42);
	m_frustrum[TOP].Normalize();
	
	m_frustrum[BACK].Set(matViewProj._14 - matViewProj._13,matViewProj._24 - matViewProj._23,matViewProj._34 - matViewProj._33,matViewProj._44 - matViewProj._43);
	m_frustrum[BACK].Normalize();
	
	m_frustrum[FRONT].Set(matViewProj._14 + matViewProj._13,matViewProj._24 + matViewProj._23,matViewProj._34 + matViewProj._33,matViewProj._44 + matViewProj._43);
	m_frustrum[FRONT].Normalize();
}

void HoeCamera::Update()
{
	matView.Camera(pos.xyz,look);
	matViewProj.Multiply(matView,matProj);
	CalculateFrustum();
}

void HoeCamera::SetPerspective( const float fov, const float zn, const float zf)
{
	matProj.PerspectiveFov( fov, 1.0f, zn, zf );
}

void HoeCamera::GetSize(int *w, int *h) const
{
	*w = (int)m_width;
	*h = (int)m_height;
}

void HoeCamera::GetViewProjMatrix(HoeMath::MATRIX * m) const
{
	m->Multiply(matView,matProj);

}

bool HoeCamera::PointInFlustrum(const VECTOR3 & point) const
{
	for(int i = 0; i < 6; i++ )
	{
		if (m_frustrum[i].Func(point) <= 0)
			return false;
	}
	return true;
}

bool HoeCamera::BoundInFlustrum(const VECTOR3 & center, const HoeMath::BoundingBox & box) const
{
	for(int i = 0; i < 6; i++ )
	{
		if (m_frustrum[i].Func(center) <= -box.ball)
			return false;
	}
	return true;
}

void HoeCamera::Pick(const float x, const float y, float * vPickRayDir, float * vPickRayOrig)
{
	static MATRIX m;
        
	// Compute the vector of the pick ray in screen space
    static VECTOR3 v;
    v.x =  ( ( ( 2.0f * x ) / m_width  ) - 1 ) / matProj._11;
    v.y = -( ( ( 2.0f * y ) / m_height ) - 1 ) / matProj._22;
    v.z =  1.0f;

        // Get the inverse view matrix
    m.Inverse(matView);

        // Transform the screen space pick ray into 3D space
    vPickRayDir[0]  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir[1]  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir[2]  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig[0] = m._41;
    vPickRayOrig[1] = m._42;
    vPickRayOrig[2] = m._43;
}


