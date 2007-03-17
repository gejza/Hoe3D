
#ifndef _HOE_MATH_DEF_H_
#define _HOE_MATH_DEF_H_

#ifndef HOE_INLINE
#define HOE_INLINE inline
#endif // HOE_INLINE

namespace HoeMath {

struct Rect;
class Matrix;

struct Vector2
{
	union {
		struct {
			float x;
			float y;
		};
		float m[2];
	};
	Vector2() { };
	Vector2(float X,float Y)
	{
		x = X;y = Y;
	}
	Vector2(float * v)
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
	HOE_INLINE void Set(float X,float Y)
	{
		x = X;y = Y;
	}
	HOE_INLINE double Magnitude(void) const;
	HOE_INLINE float MagnitudeF(void) const;

};

template<class TYPE> struct Vector2T
{
	union {
		struct {
			TYPE x;
			TYPE y;
		};
		TYPE m[2];
	};
	Vector2T() { };
	Vector2T(TYPE X,TYPE Y)
	{
		x = X;y = Y;
	}
	Vector2T(TYPE * v)
	{
		x = v[0];y = v[1];
	}
	const Vector2T operator + ( const Vector2T& v ) const
	{
		return Vector2Ex(x + v.x, y + v.y);
	}
	const Vector2T& operator = ( const Vector2T& v )
	{
		x = v.x;y = v.y;
		return *this;
	}
	bool operator == ( const Vector2T& v ) const
	{
		return (x == v.x && y == v.y);
	}
};

typedef Vector2T<int> Vector2Int;

struct Vector3
{
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float m[3];
	};

	Vector3() { };
	Vector3(float X,float Y,float Z)
	{
		x = X;y = Y;z = Z;
	}
	Vector3(float * v)
	{
		x = v[0];y = v[1]; z = v[2];
	}
	HOE_INLINE void RotateY(float angle);

	HOE_INLINE void Set(float *f)
	{
		x = f[0];
		y = f[1];
		z = f[2];
	}
	HOE_INLINE void Set(float X,float Y,float Z)
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
	Vector3 operator * ( const float f ) const
	{
		return Vector3(x * f, y * f, z * f);
	}
	Vector3 operator / ( const float f ) const
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
	HOE_INLINE void Multiply(const Vector3 &,const Matrix &);
	HOE_INLINE const Vector3 & Multiply(const Matrix &);
	/** Funkce slozi vektor z nejvetsich slozek */
	HOE_INLINE void Max(const Vector3 & v);
	/** Funkce slozi vektor z nejvetsich slozek */
	HOE_INLINE void Min(const Vector3 & v);

	Vector3 &operator *= (const Matrix &m)
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

struct Vector4
{
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float m[4];
	};

	Vector4() { };
	Vector4(const float X,const float Y,const float Z,const float W)
	{
		x = X;y = Y;z = Z;w = W;
	}
	Vector4(const float * v)
	{
		x = v[0];y = v[1]; z = v[2];w = v[3];
	}
#ifdef D3DVECTOR_DEFINED
	Vector4(const D3DVECTOR & v)
	{
		x = v.x;y = v.y; z = v.z; w = 0.f;
	}
#endif
};


struct Quat
{
	float x;
	float y;
	float z;
	float w;
	void Set(float W, float X, float Y, float Z)
	{
		x = X;y = Y;z = Z;w = W;
	}

	void Create(const Vector3 &v, const float angle)
	{
		const float a = sinf(angle * 0.5f);
		x = a * v.x;
		y = a * v.y;
		z = a * v.z;
		w = cosf(angle * 0.5f);
	}
	void Create(const float X, const float Y, const float Z, const float angle)
	{
		const float a = sinf(angle * 0.5f);
		x = a * X;
		y = a * Y;
		z = a * Z;
		w = cosf(angle * 0.5f);
	}
	HOE_INLINE double Magnitude(void) const;
	HOE_INLINE void Normalize(void);
	HOE_INLINE void GetMatrix(Matrix * m) const;
};

struct Rect
{
	float left;
	float top;
	float right;
	float bottom;
	bool IsVectorInXY(Vector3 &v)
	{
		return (v.x > left && v.x < right && v.y > top && v.y < bottom);
	}

	bool IsVectorOnXY(Vector3 &v)
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

struct Plane
{
	float a;
	float b;
	float c;
	float d;
	void Set(float A,float B,float C,float D)
	{
		a = A;b = B;c = C;d = D;
	}
	void Normalize()
	{
		const float magnitude = (float)sqrt( a * a + b * b + c * c );
		a /= magnitude;
		b /= magnitude;
		c /= magnitude;
		d /= magnitude; 
	}
	float Func(const Vector3 & p) const
	{
		return a * p.x + b * p.y + c * p.z + d;
	}
};

struct Triangle
{
	Vector3 a;
	Vector3 b;
	Vector3 c;
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

struct Line2
{
#if 1
	double a;
	double b;
	double c;
#else
	float a;
	float b;
	float c;
#endif
	void Set(const float x1,const float y1,const float x2,const float y2)
	{
		a = y2 - y1;
		b = x1 - x2;
		c = -a*x1 - b*y1;
	}
	void SetXY(const Vector2 &v1,const Vector2 &v2)
	{
		a = v2.y - v1.y;
		b = v1.x - v2.x;
		c = -a*v1.x -b*v1.y;
	}
	double Compute(const float x,const float y)
	{
		return a * x + b * y + c;
	}
	bool Intersection(Line2 &line, float& x, float& y)
	{
		//if(*this == primka)// Pøímky jsou splývající - nekoneènì mnoho spoleèných bodù
		//	return false;// Spíše by se mìlo vrátit true a nìjaký bod... záleží na použití
		//else if(JsouRovnobezne(primka))// Pøímky jsou rovnobìžné - žádný spoleèný bod
		//	return false;
		//else// Jeden spoleèný bod - prùseèík (vyhovuje souèasnì obìma rovnicím)
		{
			const double t = (a*line.b - line.a*b);
			if (t == 0)
				return false;
			x = float((b*line.c - c * line.b) / t);
			y = float(-(a*line.c - line.a * c) / t);
			return true;
		}
	}

};

template<class TYPE> struct VLineT
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

typedef VLineT<Vector2Int> VLine2Int;
typedef VLineT<Vector2> VLine2;

class Matrix
{
public:
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };
#ifdef _HOE_D3D_
	operator const D3DMATRIX*() const
	{
		return reinterpret_cast<const D3DMATRIX*>(this);
	}
	operator D3DMATRIX*()
	{
		return reinterpret_cast<D3DMATRIX*>(this);
	}
#endif
#ifdef _HOE_OPENGL_
	operator const GLfloat*() const
	{
		return reinterpret_cast<const GLfloat*>(this);
	}
#endif

	HOE_INLINE void Identity(void);
	HOE_INLINE void RotationX(const float angle);
	HOE_INLINE void RotationY(const float angle);
	HOE_INLINE void RotationZ(const float angle);
	HOE_INLINE void Scale(const float s);
	HOE_INLINE void Scale(const float sx,const float sy,const float sz);
	HOE_INLINE void Scale(const Vector3 &s);
	HOE_INLINE void Translate(const float x,const float y,const float z);
	HOE_INLINE void Translate(const Vector3 &v);
	HOE_INLINE float Inverse(const Matrix &m);
	HOE_INLINE void Adjung(const Matrix &m);
	HOE_INLINE void Transpoze(const Matrix &m);
	HOE_INLINE void Transpoze();

	HOE_INLINE void Camera(const Vector3 &pos,const Vector3 &look);
	HOE_INLINE void Ortho(const float w,const float h,const float zn,const float zf);
	HOE_INLINE void Ortho(const float l,const float r,const float b,const float t,const float zn,const float zf);
	HOE_INLINE void Perspective(const float w,const float h,const float zn,const float zf);
	HOE_INLINE void PerspectiveFov(const float fovX,const float a,const float zn,const float zf);

	HOE_INLINE void Multiply( const Matrix &a,const Matrix &b);
	HOE_INLINE void Multiply( const Matrix &m);
	HOE_INLINE void MultiplyLeft( const Matrix &m);

	void ConPrint();
};

struct BoundingBox3
{
	Vector3 min;
	Vector3 max;
	float ball;
	HOE_INLINE void Set(const Vector3 & v);
	HOE_INLINE void Add(const Vector3 & v);	
	HOE_INLINE void Set(const BoundingBox3 & b);
	HOE_INLINE void Add(const BoundingBox3 & b);
	HOE_INLINE void Compute(const Vector3 *first, dword numvert, uint stride);
};



};

#endif // _HOE_MATH_DEF_H_

