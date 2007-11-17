
#ifndef _HOE_CAMERA_H_
#define _HOE_CAMERA_H_

#include "../include/hoeinterfaces.h"
#include <hoe_math.h>
#include "map_utils.h"


class HoeCamera : public IHoeCamera
{
	MapPosition pos;
	HoeMath::Vector3v look;

	static uint m_width;
	static uint m_height;

	HoeMath::Matrix4v matView;
	HoeMath::Matrix4v matProj;
	HoeMath::Matrix4v matViewProj;

    enum FrustumSide
	{
		RIGHT = 0,
		LEFT = 1,
		BOTTOM = 2,
		TOP = 3,
		BACK = 4,
		FRONT = 5
	};
	HoeMath::Plane_v m_frustrum[6];

	void CalculateFrustum();
	void Update();

public:
	HoeCamera();
	static void SetView(uint,uint);
	void SetPerspective( const vfloat fov, const vfloat zn, const vfloat zf);

	void SetupMatrices();
	static void Setup2DMatrices(const vfloat w,const vfloat h);

	void HOEAPI Set(const HoeMath::Vector3v & pos,
		const HoeMath::Vector3v & look);
	
	void HOEAPI Pick(const vfloat x, const vfloat y, HoeMath::Vector3v * vPickRayDir, HoeMath::Vector3v * vPickRayOrig);

	static inline void sGetSize(int *w, int *h)
	{
		*w = m_width;
		*h = m_height;
	}

	void HOEAPI GetSize(int *w, int *h) const;

	inline const MapPosition & GetMapPos() const
	{
		return pos;
	}

	void GetViewProjMatrix(HoeMath::Matrix4v * m) const;

	bool PointInFlustrum(const HoeMath::Vector3v & point) const;

	bool BoundInFlustrum(const HoeMath::Vector3v & center, const HoeMath::BoundingBox3v & box) const;

	const HoeMath::Vector3v & GetLook() const { return look; }

};

#endif // _HOE_CAMERA_H_

