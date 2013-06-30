
#ifndef _HOE_MATH_DEF_H_
#define _HOE_MATH_DEF_H_

#include "hoe_fixed.h"

#ifndef HOE_INLINE
#define HOE_INLINE inline
#endif // HOE_INLINE

#ifdef UNDER_CE
#define asinf(d)        (FLOAT)asin(d)
#define acosf(d)        (FLOAT)acos(d)
#define atan2f(d1,d2)   (FLOAT)atan2(d1,d2)
#define floorf(d)       (FLOAT)floor(d)
#endif

namespace HoeMath {

struct Rect;
template<class TYPE> struct Matrix4;

template<class TYPE> struct Vector2
{
	TYPE x;
	TYPE y;
	
	Vector2() { };
	Vector2(const TYPE& X, const TYPE& Y)
	{
		x = X;y = Y;
	}
	Vector2(TYPE * v)
	{
		x = v[0];y = v[1];
	}
	const Vector2 operator + ( const Vector2& v ) const
	{
		return Vector2(x + v.x, y + v.y);
	}
	const Vector2& operator = ( const Vector2& v )
	{
		x = v.x;y = v.y;
		return *this;
	}
	HOE_INLINE void Set(TYPE X,TYPE Y)
	{
		x = X;y = Y;
	}
	bool operator == ( const Vector2& v ) const
	{
		return (x == v.x && y == v.y);
	}
    HOE_INLINE double Magnitude(void) const
    {
        return sqrt( (x * x) + (y * y));
    }

    HOE_INLINE float MagnitudeF(void) const
    {
        return sqrtf( (x * x) + (y * y));
    }

};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector2<vfloat> Vector2v;

template<class TYPE> struct Vector3
{
    TYPE x;
    TYPE y;
    TYPE z;

	Vector3() { };
	Vector3(TYPE X,TYPE Y,TYPE Z)
	{
		x = X;y = Y;z = Z;
	}
	Vector3(TYPE * v)
	{
		x = v[0];y = v[1]; z = v[2];
	}
	HOE_INLINE void RotateY(TYPE angle);

	HOE_INLINE void Set(TYPE *f)
	{
		x = f[0];
		y = f[1];
		z = f[2];
	}
	HOE_INLINE void Set(TYPE X,TYPE Y,TYPE Z)
	{
		x = X;y = Y;z = Z;
	}

	Vector3& operator = ( const Vector3& v )
	{
		x = v.x;y = v.y;z = v.z;
		return *this;
	}
	Vector3& operator += ( const Vector3& v )
	{
		x += v.x;y += v.y;z += v.z;
		return *this;
	}
	Vector3& operator -= ( const Vector3& v )
	{
		x -= v.x;y -= v.y;z -= v.z;
		return *this;
	}
	Vector3 operator - ( const Vector3& v ) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator + ( const Vector3& v ) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator * ( const TYPE f ) const
	{
		return Vector3(x * f, y * f, z * f);
	}
	Vector3 operator / ( const TYPE f ) const
	{
		return Vector3(x / f, y / f, z / f);
	}
	bool operator < ( const Vector3 & v) const
	{
		return (x < v.x && y < v.y && z < v.z);
	}
	bool operator > ( const Vector3 & v) const
	{
		return (x > v.x && y > v.y && z > v.z);
	}
	bool operator == (const Vector3 & v) const
	{
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}

	HOE_INLINE double Magnitude(void) const;
	HOE_INLINE float MagnitudeF(void) const;
	HOE_INLINE const Vector3 & Normalize(void);
	HOE_INLINE void Multiply(const Vector3 &,const Matrix4<TYPE> &);
	HOE_INLINE const Vector3 & Multiply(const Matrix4<TYPE> &);
	/** Funkce slozi vektor z nejvetsich slozek */
	HOE_INLINE void Max(const Vector3 & v);
	/** Funkce slozi vektor z nejmensich slozek */
	HOE_INLINE void Min(const Vector3 & v);

	Vector3 &operator *= (const Matrix4<TYPE> &m)
	{
		Multiply(m);
		return *this;
	}
#ifdef D3DVECTOR_DEFINED
	operator const D3DVECTOR()
	{
		return *reinterpret_cast<D3DVECTOR*>(this);
	}
#endif
};

typedef Vector3<float> Vector3f;
typedef Vector3<fixed> Vector3fx;
typedef Vector3<int> Vector3i;
typedef Vector3<double> Vector3d;
typedef Vector3<vfloat> Vector3v;

template <class TYPE> struct Vector4
{
    TYPE x;
    TYPE y;
    TYPE z;
    TYPE w;

	Vector4() { };
	Vector4(const TYPE X,const TYPE Y,const TYPE Z,const TYPE W)
	{
		x = X;y = Y;z = Z;w = W;
	}
	Vector4(const TYPE * v)
	{
		x = v[0];y = v[1]; z = v[2];w = v[3];
	}
#ifdef D3DVECTOR_DEFINED
	Vector4(const D3DVECTOR & v)
	{
		x = v.x;y = v.y; z = v.z; w = 0.f;
	}
#endif
	operator const TYPE* () const
	{
		return &x;
	}
};

typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;
typedef Vector4<vfloat> Vector4v;

template<class TYPE> struct Quat
{
	TYPE x;
	TYPE y;
	TYPE z;
	TYPE w;
	void Set(TYPE W, TYPE X, TYPE Y, TYPE Z)
	{
		x = X;y = Y;z = Z;w = W;
	}

	void Create(const Vector3<TYPE> &v, const TYPE angle)
	{
		const vfloat a = sinf(angle * 0.5f);
		x = a * v.x;
		y = a * v.y;
		z = a * v.z;
		w = cosf(angle * 0.5f);
	}
	void Create(const TYPE X, const TYPE Y, const TYPE Z, const TYPE angle)
	{
		const float a = sinf(angle * 0.5f);
		x = a * X;
		y = a * Y;
		z = a * Z;
		w = cosf(angle * 0.5f);
	}
	HOE_INLINE double Magnitude(void) const;
	HOE_INLINE void Normalize(void);
	HOE_INLINE void GetMatrix(Matrix4<TYPE> * m) const;
};

typedef Quat<float> Quatf;
typedef Quat<vfloat> Quatv;
typedef Quat<double> Quatd;
typedef Quat<int> Quati;

struct Rect
{
	float left;
	float top;
	float right;
	float bottom;
	bool IsVectorInXY(Vector3f &v)
	{
		return (v.x > left && v.x < right && v.y > top && v.y < bottom);
	}

	bool IsVectorOnXY(Vector3f &v)
	{
		// return (v.x+TOL >= left && v.x-TOL <= right && v.y+TOL >= top && v.y-TOL <= bottom);
		return (v.x >= left && v.x <= right && v.y >= top && v.y <= bottom);
	}
	bool IsIn(const float x,const float y)
	{
		return (x >= left && x <= right && y >= top && y <= bottom);
	}
	bool IsIn(const Rect &in,Rect & out)
	{
		if (right < in.left || left > in.right || top > in.bottom || bottom < in.top)
			return false;
		out.left = (in.left < left) ? left:in.left;
		out.right = (in.right < right) ? in.right : right;
		out.top = (in.top > top) ? in.top : top;
		out.bottom = (in.bottom < bottom) ? in.bottom : bottom;
		return ((out.bottom >= out.top) && (out.left <= out.right));
	}
	void Set(const float l,const float t,const float r,const float b)
	{
		top = t;left = l;right = r;bottom = b;
	}
	void More(const float x,const float y)
	{
		if (x < left) left = x;
		if (x > right) right = x;
		if (y < top) top = y;
		if (y > bottom) bottom = y;
	}
	void More(Rect &r)
	{
		if (r.left < left) left = r.left;
		if (r.right > right) right = r.right;
		if (r.top < top) top = r.top;
		if (r.bottom > bottom) bottom = r.bottom;
	}
	void GetVector(int i,float &x,float y)
	{
		switch (i % 4)
		{
		case 0:
			x = right;y=top;break;
		case 1:
			x = left;y = top;break;
		case 2:
			x = left;y = bottom;break;
		case 3:
			x = right;y = bottom;break;
		};
	}
};

template<class TYPE> struct Plane
{
	TYPE a;
	TYPE b;
	TYPE c;
	TYPE d;
	void Set(TYPE A,TYPE B,TYPE C,TYPE D)
	{
		a = A;b = B;c = C;d = D;
	}
	void Normalize()
	{
		const TYPE magnitude = (TYPE)sqrt( a * a + b * b + c * c );
		if (magnitude == 0) //TODO
			return;
		a /= magnitude;
		b /= magnitude;
		c /= magnitude;
		d /= magnitude; 
	}
	TYPE Func(const Vector3<TYPE> & p) const
	{
		return a * p.x + b * p.y + c * p.z + d;
	}
};

typedef Plane<vfloat> Plane_v;
typedef Plane<float> Plane_f;
typedef Plane<int> Plane_i;

template<class TYPE> struct Triangle
{
	Vector3<TYPE> a;
	Vector3<TYPE> b;
	Vector3<TYPE> c;
	void SignXY(Rect &rect)
	{
		rect.More(a.x,a.y);
		rect.More(b.x,b.y);
		rect.More(c.x,c.y);
	}
	int NumVectorsXY(Rect &rect)
	{
		int n=0;
		if (rect.IsVectorOnXY(a)) n++;
		if (rect.IsVectorOnXY(b)) n++;
		if (rect.IsVectorOnXY(c)) n++;
		return n;
	}
};

template<class TYPE> struct Line2
{
	TYPE a;
	TYPE b;
	TYPE c;
	void Set(const TYPE x1,const TYPE y1,const TYPE x2,const TYPE y2)
	{
		a = y2 - y1;
		b = x1 - x2;
		c = -a*x1 - b*y1;
	}
	void SetXY(const Vector2<TYPE> &v1,const Vector2<TYPE> &v2)
	{
		a = v2.y - v1.y;
		b = v1.x - v2.x;
		c = -a*v1.x -b*v1.y;
	}
	double Compute(const TYPE x,const TYPE y)
	{
		return a * x + b * y + c;
	}
	bool Intersection(Line2 &line, TYPE& x, TYPE& y)
	{
		//if(*this == primka)// P��mky jsou spl�vaj�c� - nekone�n� mnoho spole�n�ch bod�
		//	return false;// Sp�e by se m�lo vr�tit true a n�jak� bod... z�le�� na pou�it�
		//else if(JsouRovnobezne(primka))// P��mky jsou rovnob�n� - ��dn� spole�n� bod
		//	return false;
		//else// Jeden spole�n� bod - pr�se��k (vyhovuje sou�asn� ob�ma rovnic�m)
		{
			const double t = (a*line.b - line.a*b);
			if (t == 0)
				return false;
			x = TYPE((b*line.c - c * line.b) / t);
			y = TYPE(-(a*line.c - line.a * c) / t);
			return true;
		}
	}

};

typedef Line2<int> Line2i;
typedef Line2<float> Line2f;
typedef Line2<double> Line2d;
typedef Line2<fixed> Line2fx;
typedef Line2<vfloat> Line2v;

template<class TYPE> struct SegmentLine
{
	TYPE a;
	TYPE b;
	bool IfCanConnect(TYPE &next)
	{
		if(a == next)
		{
			next = b;
			return true;
		}
		if(b == next)
		{
			next = a;
			return true;
		}
		return false;
	}
};

typedef SegmentLine<Vector2i> SegmentLine2i;
typedef SegmentLine<Vector2f> SegmentLine2f;

template<class TYPE> struct Matrix4
{
public:
    TYPE        _11, _12, _13, _14;
    TYPE        _21, _22, _23, _24;
    TYPE        _31, _32, _33, _34;
    TYPE        _41, _42, _43, _44;

	operator const TYPE*() const
	{
		return reinterpret_cast<const TYPE*>(this);
	}
	operator TYPE*()
	{
		return reinterpret_cast<TYPE*>(this);
	}

	HOE_INLINE void Identity(void);
	HOE_INLINE void RotationX(const TYPE angle);
	HOE_INLINE void RotationY(const TYPE angle);
	HOE_INLINE void RotationZ(const TYPE angle);
	HOE_INLINE void Scale(const TYPE s);
	HOE_INLINE void Scale(const TYPE sx,const TYPE sy,const TYPE sz);
	HOE_INLINE void Scale(const Vector3<TYPE> &s);
	HOE_INLINE void Translate(const TYPE x,const TYPE y,const TYPE z);
	HOE_INLINE void Translate(const Vector3<TYPE> &v);
	HOE_INLINE TYPE Inverse(const Matrix4 &m);
	HOE_INLINE void Adjung(const Matrix4 &m);
	HOE_INLINE void Transpoze(const Matrix4 &m);
	HOE_INLINE void Transpoze();

	HOE_INLINE void Camera(const Vector3<TYPE> &pos,const Vector3<TYPE> &look);
	HOE_INLINE void Ortho(const TYPE w,const TYPE h,const TYPE zn,const TYPE zf);
	HOE_INLINE void Ortho(const TYPE l,const TYPE r,const TYPE b,const TYPE t,const TYPE zn,const TYPE zf);
	HOE_INLINE void Perspective(const TYPE w,const TYPE h,const TYPE zn,const TYPE zf);
	HOE_INLINE void PerspectiveFov(const TYPE fovX,const TYPE a,const TYPE zn,const TYPE zf);

	HOE_INLINE void Multiply( const Matrix4 &a,const Matrix4 &b);
	HOE_INLINE void Multiply( const Matrix4 &m);
	HOE_INLINE void MultiplyLeft( const Matrix4 &m);

	void ConPrint();
};

typedef Matrix4<float> Matrix4f;
typedef Matrix4<fixed> Matrix4fx;
typedef Matrix4<vfloat> Matrix4v;

template<class TYPE> struct BoundingBox3
{
	Vector3<TYPE> min;
	Vector3<TYPE> max;
	TYPE ball;
	HOE_INLINE void Set(const Vector3<TYPE> & v);
	HOE_INLINE void Add(const Vector3<TYPE> & v);	
	HOE_INLINE void Set(const BoundingBox3 & b);
	HOE_INLINE void Add(const BoundingBox3 & b);
	HOE_INLINE void Compute(const Vector3<TYPE> *first, dword numvert, uint stride);
};

typedef BoundingBox3<float> BoundingBox3f;
typedef BoundingBox3<vfloat> BoundingBox3v;

};

#endif // _HOE_MATH_DEF_H_

