
#ifndef _HOE_MATH_H_
#define _HOE_MATH_H_

#include <math.h>

#ifdef __cplusplus

#include "hoe_types.h"
#include "hoe_math_def.h"

namespace HoeMath {


#define TOL 0.1f

#define VEC3(array)  ((HoeMath::Vector3f*)array)

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
HOE_INLINE float HoeDot(const Vector3f &vec1,const Vector3f &vec2);
HOE_INLINE float HoeDot(const Vector2f &vec1,const Vector2f &vec2);
HOE_INLINE void HoeCross(const Vector3f &vec1, const Vector3f &vec2,Vector3f &cross);
HOE_INLINE const Vector3f HoeCross(const Vector3f &vec1, const Vector3f &vec2);
HOE_INLINE double HoeAngleBetweenVectors(const Vector3f &v1, const Vector3f &v2);
HOE_INLINE float HoeAngleBetweenVectorsF(const Vector2f &v1, const Vector2f &v2);

HOE_INLINE bool HoeInsidePolygon(Line2f *,int num_lines,const float x,const float y,int i1,int i2);

HOE_INLINE bool HoePointInBox(Vector3f &p,float minx,float miny, float minz, float maxx, float maxy, float maxz);
HOE_INLINE void HoePlaneNormal(const Vector3f &a, const Vector3f &b, const Vector3f &c,Vector3f &normal);
HOE_INLINE float HoePlaneDistance(Vector3f &normal, Vector3f &point);
HOE_INLINE float HoeDistance(Vector2f &a, Vector2f &b);
HOE_INLINE float HoeDistance(Vector3f &a, Vector3f &b);
HOE_INLINE int HoeClassifySphere(Vector3f &pos, 
				   Vector3f &normal, Vector3f &a, float radius, float &distance);
HOE_INLINE bool HoeInsidePolygon(const Vector3f &intersection, const Vector3f &a,const Vector3f &b,const Vector3f &c);
HOE_INLINE void HoeGetCollisionOffset(Vector3f &normal, float radius, float distance,Vector3f &offset);
HOE_INLINE bool HoeEdgePlanesCollision(Vector3f &center, 
						 Vector3f &a,Vector3f &b,Vector3f &c, Vector3f &normal, float radius);
void MinMaxF(const float * src, uint count, uint stride, float * min, float * max);
void MinMaxV2(const Vector2f * src, uint count, uint stride, Vector2f * min, Vector2f * max);
void MinMaxV3(const Vector3f * src, uint count, uint stride, Vector3f * min, Vector3f * max);

// zaokrouhlovaci funkce
HOE_INLINE float UpperRound(float f);
////////////
// matematik func
#define _hoe_cot cot

#include "hoe_math.inl"

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_MATH_H_
