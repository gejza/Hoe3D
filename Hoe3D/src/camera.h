
#ifndef _HOE_CAMERA_H_
#define _HOE_CAMERA_H_

#include "../include/hoeinterfaces.h"
#include <hoe_math.h>
#include "map_utils.h"


class HoeCamera : public IHoeCamera
{
	MapPosition pos;
	HoeMath::VECTOR3 look;

	static uint m_width;
	static uint m_height;

	HoeMath::MATRIX matView;
	HoeMath::MATRIX matProj;
	HoeMath::MATRIX matViewProj;

    enum FrustumSide
	{
		RIGHT = 0,
		LEFT = 1,
		BOTTOM = 2,
		TOP = 3,
		BACK = 4,
		FRONT = 5
	};
	HoeMath::PLANE m_frustrum[6];

	void CalculateFrustum();
	void Update();

public:
	HoeCamera();
	static void SetView(uint,uint);
	void SetPerspective( const float fov, const float zn, const float zf);

	void SetupMatrices();
	static void Setup2DMatrices(const float w,const float h);

	void HOEAPI Set(const HoeMath::VECTOR3 & pos,
		const HoeMath::VECTOR3 & look);
	
	void HOEAPI Pick(const float x, const float y, HoeMath::VECTOR3 * vPickRayDir, HoeMath::VECTOR3 * vPickRayOrig);

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

	void GetViewProjMatrix(HoeMath::MATRIX * m) const;

	bool PointInFlustrum(const HoeMath::VECTOR3 & point) const;

	bool BoundInFlustrum(const HoeMath::VECTOR3 & center, const HoeMath::BoundingBox & box) const;
};

#endif // _HOE_CAMERA_H_

