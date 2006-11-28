
#ifndef _HOE_CAMERA_H_
#define _HOE_CAMERA_H_

#include "../include/hoeinterfaces.h"
#include <hoe_math.h>
#include "map_utils.h"


class HoeCamera : public IHoeCamera
{
	MapPosition pos;
	HoeMath::Vector3 look;

	static uint m_width;
	static uint m_height;

	HoeMath::Matrix matView;
	HoeMath::Matrix matProj;
	HoeMath::Matrix matViewProj;

    enum FrustumSide
	{
		RIGHT = 0,
		LEFT = 1,
		BOTTOM = 2,
		TOP = 3,
		BACK = 4,
		FRONT = 5
	};
	HoeMath::Plane m_frustrum[6];

	void CalculateFrustum();
	void Update();

public:
	HoeCamera();
	static void SetView(uint,uint);
	void SetPerspective( const float fov, const float zn, const float zf);

	void SetupMatrices();
	static void Setup2DMatrices(const float w,const float h);

	void HOEAPI Set(const HoeMath::Vector3 & pos,
		const HoeMath::Vector3 & look);
	
	void HOEAPI Pick(const float x, const float y, HoeMath::Vector3 * vPickRayDir, HoeMath::Vector3 * vPickRayOrig);

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

	void GetViewProjMatrix(HoeMath::Matrix * m) const;

	bool PointInFlustrum(const HoeMath::Vector3 & point) const;

	bool BoundInFlustrum(const HoeMath::Vector3 & center, const HoeMath::BoundingBox3 & box) const;

	const HoeMath::Vector3 & GetLook() const { return look; }

};

#endif // _HOE_CAMERA_H_

