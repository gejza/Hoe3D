
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include <hoe_math.h>
#include "config.h"
#include "camera.h"
#include "hoe_time.h"
#include "sound.h"

using namespace HoeMath;

uint HoeCamera::m_width = 0;
uint HoeCamera::m_height = 0;

HoeCamera::HoeCamera()
{
	//pos.part = NULL;
	pos.xyz = Vector3(0,0,-100);
	look = Vector3(0,0,1);
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
	D3DDevice()->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&matView );
	D3DDevice()->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&matProj );
#endif
#ifdef _HOE_OPENGL_
	glMatrixMode(GL_PROJECTION);// Zvolí projekèní matici
	glLoadMatrixf((const GLfloat *)matViewProj.m);
	
	glMatrixMode(GL_MODELVIEW);// Zvolí matici modelview
	glLoadIdentity();

#endif

}

void HoeCamera::Setup2DMatrices(const float w,const float h)
{
	Matrix matWorld;
	matWorld.Ortho( 0,(w > 0) ? w:m_width,(h > 0) ? h:m_height,0,-1,1); 

#ifdef _HOE_D3D_
	Matrix matView;
	matView.Identity();
	D3DDevice()->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&matWorld );
	D3DDevice()->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&matView );
	D3DDevice()->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&matView );
#endif
#ifdef _HOE_D3DM_
	Matrix matView;
	matView.Identity();
	D3DDevice()->SetTransform( D3DMTS_WORLD, (const D3DMMATRIX*)&matWorld, D3DMFMT_D3DMVALUE_FLOAT );
	D3DDevice()->SetTransform( D3DMTS_VIEW, (const D3DMMATRIX*)&matView, D3DMFMT_D3DMVALUE_FLOAT );
	D3DDevice()->SetTransform( D3DMTS_PROJECTION, (const D3DMMATRIX*)&matView,D3DMFMT_D3DMVALUE_FLOAT );
#endif
#ifdef _HOE_OPENGL_
	glMatrixMode(GL_PROJECTION);// Zvolí projekèní matici
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);// Zvolí matici modelview
	glLoadMatrixf((GLfloat*)&matWorld);
#endif
}

const float scale = 0.1f;

void HoeCamera::Set(const HoeMath::Vector3 & p, const HoeMath::Vector3 & l)
{
	//this->pos.part = (HoeMapPart *)part;
	this->pos.xyz = p;
	look = l;

	// set sound
	#ifdef _HOE_DS8_
	GetSound()->GetListener()->SetPosition(p.x*scale,p.y*scale,p.z*scale,DS3D_DEFERRED);
	HoeMath::Vector3 f = l;
	f.Normalize();
	HoeMath::Vector3 f2 = l;
	f2.Normalize();
	HoeMath::Vector3 top;
	HoeMath::HoeCross(f, f2, top);
	GetSound()->GetListener()->SetOrientation(f.x,f.y,f.z,top.x,top.y,top.z,DS3D_DEFERRED);
	GetSound()->GetListener()->CommitDeferredSettings();
#endif // _HOE_DS8_
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

void HoeCamera::GetViewProjMatrix(HoeMath::Matrix * m) const
{
	m->Multiply(matView,matProj);

}

bool HoeCamera::PointInFlustrum(const Vector3 & point) const
{
	for(int i = 0; i < 6; i++ )
	{
		if (m_frustrum[i].Func(point) <= 0)
			return false;
	}
	return true;
}

bool HoeCamera::BoundInFlustrum(const Vector3 & center, const HoeMath::BoundingBox3 & box) const
{
	for(int i = 0; i < 6; i++ )
	{
		if (m_frustrum[i].Func(center) <= -box.ball)
			return false;
	}
	return true;
}

void HoeCamera::Pick(const float x, const float y, HoeMath::Vector3 * vPickRayDir, HoeMath::Vector3 * vPickRayOrig)
{
	static Matrix m;
        
	// Compute the vector of the pick ray in screen space
    static Vector3 v;
    v.x =  ( ( ( 2.0f * x ) / m_width  ) - 1 ) / matProj._11;
    v.y = -( ( ( 2.0f * y ) / m_height ) - 1 ) / matProj._22;
    v.z =  1.0f;

        // Get the inverse view Matrix
    m.Inverse(matView);

        // Transform the screen space pick ray into 3D space
    vPickRayDir->x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir->y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir->z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig->x = m._41;
    vPickRayOrig->y = m._42;
    vPickRayOrig->z = m._43;
}


