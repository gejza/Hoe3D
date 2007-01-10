
#ifndef _HOE_MATH_H_
#define _HOE_MATH_H_

#include <math.h>

#ifdef __cplusplus

#include "hoe_types.h"
#include "hoe_math_def.h"

namespace HoeMath {


#define TOL 0.1f

#define VEC3(array)  ((HoeMath::Vector3*)array)

static const double HOE_PI = 3.1415926535897932;
static const double HOE_2PI = (2*HOE_PI);
static const float HOE_PIF = ((float)  3.141592654f);
static const float HOE_2PIF = (2*HOE_PIF);


#define HOEToRadian( degree ) ((degree) * (HOE_PI / 180.0f))
#define HOEToDegree( radian ) ((radian) * (180.0f / HOE_PI))

#define HOEToFirstInterval( angle ) ((angle) - (((int)(angle/HOE_PIF)) * HOE_PIF))

// colis
#define P_BEHIND		0
#define P_INTERSECTS	1
#define P_FRONT		2

#define MATCH_FACTOR 0.9

/** Funkce pro vypocet vektoroveho souctu */
HOE_INLINE float HoeDot(const Vector3 &vec1,const Vector3 &vec2);
HOE_INLINE float HoeDot(const Vector2 &vec1,const Vector2 &vec2);
HOE_INLINE void HoeCross(const Vector3 &vec1, const Vector3 &vec2,Vector3 &cross);
HOE_INLINE const Vector3 HoeCross(const Vector3 &vec1, const Vector3 &vec2);
HOE_INLINE double HoeAngleBetweenVectors(const Vector3 &v1, const Vector3 &v2);
HOE_INLINE float HoeAngleBetweenVectorsF(const Vector2 &v1, const Vector2 &v2);

HOE_INLINE bool HoeInsidePolygon(Line2 *,int num_lines,const float x,const float y,int i1,int i2);

HOE_INLINE bool HoePointInBox(Vector3 &p,float minx,float miny, float minz, float maxx, float maxy, float maxz);
HOE_INLINE void HoePlaneNormal(const Vector3 &a, const Vector3 &b, const Vector3 &c,Vector3 &normal);
HOE_INLINE float HoePlaneDistance(Vector3 &normal, Vector3 &point);
HOE_INLINE float HoeDistance(Vector2 &a, Vector2 &b);
HOE_INLINE float HoeDistance(Vector3 &a, Vector3 &b);
HOE_INLINE int HoeClassifySphere(Vector3 &pos, 
				   Vector3 &normal, Vector3 &a, float radius, float &distance);
HOE_INLINE bool HoeInsidePolygon(const Vector3 &intersection, const Vector3 &a,const Vector3 &b,const Vector3 &c);
HOE_INLINE void HoeGetCollisionOffset(Vector3 &normal, float radius, float distance,Vector3 &offset);
HOE_INLINE bool HoeEdgePlanesCollision(Vector3 &center, 
						 Vector3 &a,Vector3 &b,Vector3 &c, Vector3 &normal, float radius);
void MinMaxF(const float * src, uint count, uint stride, float * min, float * max);
void MinMaxV2(const Vector2 * src, uint count, uint stride, Vector2 * min, Vector2 * max);
void MinMaxV3(const Vector3 * src, uint count, uint stride, Vector3 * min, Vector3 * max);

// zaokrouhlovaci funkce
HOE_INLINE float UpperRound(float f);
////////////
// matematik func
#define _hoe_cot cot

#include "hoe_math.inl"

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_MATH_H_
