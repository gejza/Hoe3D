
#ifndef _HOE_MATH_H_
#define _HOE_MATH_H_

#include <math.h>

#ifdef __cplusplus

#include "hoe_types.h"
#include "hoe_math_def.h"

namespace HoeMath {


#define TOL 0.1f

#define VEC3(array)  ((HoeMath::VECTOR3*)array)

#define HOE_PI	3.1415926535897932
#define HOE_2PI (2*HOE_PI)
#define HOE_PIF    ((float)  3.141592654f)
#define HOE_2PIF   (2*HOE_PIF)


#define HOEToRadian( degree ) ((degree) * (HOE_PI / 180.0f))
#define HOEToDegree( radian ) ((radian) * (180.0f / HOE_PI))

#define HOEToFirstInterval( angle ) ((angle) - (((int)(angle/HOE_PIF)) * HOE_PIF))

// colis
#define P_BEHIND		0
#define P_INTERSECTS	1
#define P_FRONT		2

#define MATCH_FACTOR 0.9

/** Funkce pro vypocet vektoroveho souctu */
HOE_INLINE float HoeDot(const VECTOR3 &vec1,const VECTOR3 &vec2);
HOE_INLINE void HoeCross(const VECTOR3 &vec1, const VECTOR3 &vec2,VECTOR3 &cross);
HOE_INLINE double HoeAngleBetweenVectors(const VECTOR3 &v1, const VECTOR3 &v2);

HOE_INLINE bool HoeInsidePolygon(LINE2D *,int num_lines,const float x,const float y,int i1,int i2);

HOE_INLINE bool HoePointInBox(VECTOR3 &p,float minx,float miny, float minz, float maxx, float maxy, float maxz);
HOE_INLINE void HoePlaneNormal(const VECTOR3 &a, const VECTOR3 &b, const VECTOR3 &c,VECTOR3 &normal);
HOE_INLINE float HoePlaneDistance(VECTOR3 &normal, VECTOR3 &point);
HOE_INLINE float HoeDistance(VECTOR2 &a, VECTOR2 &b);
HOE_INLINE float HoeDistance(VECTOR3 &a, VECTOR3 &b);
HOE_INLINE int HoeClassifySphere(VECTOR3 &pos, 
				   VECTOR3 &normal, VECTOR3 &a, float radius, float &distance);
HOE_INLINE bool HoeInsidePolygon(const VECTOR3 &intersection, const VECTOR3 &a,const VECTOR3 &b,const VECTOR3 &c);
HOE_INLINE void HoeGetCollisionOffset(VECTOR3 &normal, float radius, float distance,VECTOR3 &offset);
HOE_INLINE bool HoeEdgePlanesCollision(VECTOR3 &center, 
						 VECTOR3 &a,VECTOR3 &b,VECTOR3 &c, VECTOR3 &normal, float radius);
////////////
// matematik func
#define _hoe_cot cot

#include "hoe_math.inl"

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_MATH_H_
