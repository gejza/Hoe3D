
#ifndef _HE_MATH_INL__
#define _HE_MATH_INL__

HOE_INLINE float UpperRound(float f)
{
	int i = (int)f;
	if (((float)i)==f)
		return f;
	else
		return (float)(i+1);
}

// Matrix4 & vector

//////// vector ////////////////////////////////////////

//// 3

template<class TYPE> HOE_INLINE void Vector3<TYPE>::RotateY(TYPE angle)
{
	register TYPE s,c;
    sincosf(angle,&s,&c);
	const register TYPE tx = c*x + s*z;
	z = -s*x + c*z;
	x = tx;
}

template<class TYPE> HOE_INLINE double Vector3<TYPE>::Magnitude(void) const
{
	return sqrt( (x * x) + (y * y) + (z * z) );
}

template<> HOE_INLINE float Vector3v::MagnitudeF(void) const
{
	return sqrtf( (x * x) + (y * y) + (z * z) );
}

template<class TYPE> HOE_INLINE const Vector3<TYPE> & Vector3<TYPE>::Normalize(void)
{
	TYPE magnitude = (TYPE)Magnitude();			

	x /= magnitude;							
	y /= magnitude;							
	z /= magnitude;						
	
	return *this;
}

template<> HOE_INLINE void Vector3v::Multiply(const Vector3v &v,const Matrix4v &m)
{
	x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
}

template<> HOE_INLINE const Vector3v &  Vector3v::Multiply(const Matrix4v &m)
{
	Vector3v v = *this;
	Multiply(v,m);
	return *this;
}

template<> HOE_INLINE void Vector3v::Max(const Vector3v & v)
{
	if (v.x > x) x = v.x;
	if (v.y > y) y = v.y;
	if (v.z > z) z = v.z;
}

template<> HOE_INLINE void Vector3v::Min(const Vector3v & v)
{
	if (v.x < x) x = v.x;
	if (v.y < y) y = v.y;
	if (v.z < z) z = v.z;
}

//////// quaternions ///////////////////////////////////

template<> HOE_INLINE double Quatv::Magnitude(void) const
{
	return sqrt( w*w + x*x + y*y + z*z);
}

template<> HOE_INLINE void Quatv::Normalize(void)
{
	vfloat mag = (vfloat)Magnitude();
	w = w / mag;
	x = x / mag;
	y = y / mag;
	z = z / mag;
}

template<> HOE_INLINE void Quatv::GetMatrix(Matrix4v * m) const 
{
	m->_11  = 1 - 2 * ( y*y + z*z );
    m->_12  =     2 * ( x*y - z*w );
    m->_13  =     2 * ( x*z + y*w );    
	m->_21  =     2 * ( x*y + z*w );    
	m->_22  = 1 - 2 * ( x*x + z*z );    
	m->_23  =     2 * ( y*z - x*w );    
	m->_31  =     2 * ( x*z - y*w );    
	m->_32  =     2 * ( y*z + x*w );    
	m->_33 = 1 - 2 * ( x*x + y*y );    
	m->_14  = m->_24 = m->_34 = m->_41 = m->_42 = m->_43 = 0.f;    
	m->_44 = 1.f;
}

//////// Matrix4 ////////////////////////////////////////
template<> HOE_INLINE void Matrix4v::Identity(void)
{
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f;
}

template<> HOE_INLINE void Matrix4v::RotationX(const vfloat angle)
{
	_12 = _13 = _14 = 0.0f;   
	_21 = _24 = 0.0f;	
	_31 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _44 = 1.0f;
	_22 = _33 = cosf( angle );
	_23 = -(_32 = sinf( angle ));
}


template<> HOE_INLINE void Matrix4v::RotationY(const vfloat angle)
{
    
    _12 = _14 = 0.0f;	
	_21 = _23 = _24 = 0.0f;
	_32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_22 = _44 = 1.0f;
	_11 = _33 = cosf( angle );
	_13 = -(_31 = sinf( angle ));

}

template<> HOE_INLINE void Matrix4v::RotationZ(const vfloat angle)
{
    
    _13 = _14 = 0.0f;
	_23 = _24 = 0.0f;	
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_33 = _44 = 1.0f;
	_11 = _22 = (vfloat)cosf( angle );
	_12 = -(_21 = (vfloat)sinf( angle ));

}

template<> HOE_INLINE void Matrix4v::Scale(const vfloat s)
{
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f; 
	_11 = _22 = _33 = s;
	_44 = 1.0f;

}

template<> HOE_INLINE void Matrix4v::Scale(const vfloat sx,const vfloat sy,const vfloat sz)
{
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f; 
	_11 = sx;
	_22 = sy;
	_33 = sz;
	_44 = 1.0f;

}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Scale(const Vector3<TYPE> &s)
{
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f; 
	_11 = s.x;
	_22 = s.y;
	_33 = s.z;
	_44 = 1.0f;

}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Translate(const TYPE x,const TYPE y,const TYPE z)
{
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = 0.0f;
    _21 = _23 = _24 = 0.0f;
    _31 = _32 = _34 = 0.0f;
	_41 = x;
	_42 = y;
	_43 = z;

}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Translate(const Vector3<TYPE> &v)
{
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = 0.0f;
    _21 = _23 = _24 = 0.0f;
    _31 = _32 = _34 = 0.0f;
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Adjung(const Matrix4<TYPE> &m)
{
	_11 = m._22 * m._33 * m._44 + m._23 * m._34 * m._42 + m._32 * m._43 * m._24 - m._24 * m._33 * m._42 - m._23 * m._32 * m._44 - m._22 * m._34 * m._43;
	_12 = - m._12 * m._33 * m._44 - m._13 * m._34 * m._42 - m._32 * m._43 * m._14 + m._14 * m._33 * m._42 + m._13 * m._32 * m._44 + m._12 * m._34 * m._43;
	_13 = m._12 * m._23 * m._44 + m._13 * m._24 * m._42 + m._22 * m._43 * m._14 - m._14 * m._23 * m._42 - m._13 * m._22 * m._44 - m._12 * m._24 * m._43;
	_14 = - m._12 * m._23 * m._34 - m._13 * m._24 * m._32 - m._22 * m._33 * m._14 + m._14 * m._23 * m._32 + m._13 * m._22 * m._34 + m._12 * m._24 * m._33;
	_21 = - m._21 * m._33 * m._44 - m._23 * m._34 * m._41 - m._31 * m._43 * m._24 + m._24 * m._33 * m._41 + m._23 * m._31 * m._44 + m._21 * m._34 * m._43;
	_22 = m._11 * m._33 * m._44 + m._13 * m._34 * m._41 + m._31 * m._43 * m._14 - m._14 * m._33 * m._41 - m._13 * m._31 * m._44 - m._11 * m._34 * m._43;
	_23 = - m._11 * m._23 * m._44 - m._13 * m._24 * m._41 - m._21 * m._43 * m._14 + m._14 * m._23 * m._41 + m._13 * m._21 * m._44 + m._11 * m._24 * m._43;
	_24 = m._11 * m._23 * m._34 + m._13 * m._24 * m._31 + m._21 * m._33 * m._14 - m._14 * m._23 * m._31 - m._13 * m._21 * m._34 - m._11 * m._24 * m._33;
	_31 = m._21 * m._32 * m._44 + m._22 * m._34 * m._41 + m._31 * m._42 * m._24 - m._24 * m._32 * m._41 - m._22 * m._31 * m._44 - m._21 * m._34 * m._42;
	_32 = - m._11 * m._32 * m._44 - m._12 * m._34 * m._41 - m._31 * m._42 * m._14 + m._14 * m._32 * m._41 + m._12 * m._31 * m._44 + m._11 * m._34 * m._42;
	_33 = m._11 * m._22 * m._44 + m._12 * m._24 * m._41 + m._21 * m._42 * m._14 - m._14 * m._22 * m._41 - m._12 * m._21 * m._44 - m._11 * m._24 * m._42;
	_34 = - m._11 * m._22 * m._34 - m._12 * m._24 * m._31 - m._21 * m._32 * m._14 + m._14 * m._22 * m._31 + m._12 * m._21 * m._34 + m._11 * m._24 * m._32;
	_41 = - m._21 * m._32 * m._43 - m._22 * m._33 * m._41 - m._31 * m._42 * m._23 + m._23 * m._32 * m._41 + m._22 * m._31 * m._43 + m._21 * m._33 * m._42;
	_42 = m._11 * m._32 * m._43 + m._12 * m._33 * m._41 + m._31 * m._42 * m._13 - m._13 * m._32 * m._41 - m._12 * m._31 * m._43 - m._11 * m._33 * m._42;
	_43 = - m._11 * m._22 * m._43 - m._12 * m._23 * m._41 - m._21 * m._42 * m._13 + m._13 * m._22 * m._41 + m._12 * m._21 * m._43 + m._11 * m._23 * m._42;
	_44 = m._11 * m._22 * m._33 + m._12 * m._23 * m._31 + m._21 * m._32 * m._13 - m._13 * m._22 * m._31 - m._12 * m._21 * m._33 - m._11 * m._23 * m._32;

}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Transpoze(const Matrix4<TYPE> &m)
{
	_11 = m._11;_12 = m._21; _13 = m._31; _14 = m._41;
	_21 = m._12;_22 = m._22; _23 = m._32; _24 = m._42;
	_31 = m._13;_32 = m._23; _33 = m._33; _34 = m._43;
	_41 = m._14;_42 = m._24; _43 = m._34; _44 = m._44;
}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Transpoze()
{
	Matrix4 m = *this;
	Transpoze(m);
}

template<class TYPE> HOE_INLINE TYPE Matrix4<TYPE>::Inverse( const Matrix4<TYPE> &m)
{
	Adjung(m);

	const TYPE det = m._11 * _11 + m._12 * _21 + m._13 * _31 + m._14 * _41;

	if (det == 0)
		return 0;

	_11 = _11 / det;
	_12 = _12 / det;
	_13 = _13 / det;
	_14 = _14 / det;
	_21 = _21 / det;
	_22 = _22 / det;
	_23 = _23 / det;
	_24 = _24 / det;
	_31 = _31 / det;
	_32 = _32 / det;
	_33 = _33 / det;
	_34 = _34 / det;
	_41 = _41 / det;
	_42 = _42 / det;
	_43 = _43 / det;
	_44 = _44 / det;

	return det;
}

template<class TYPE> HOE_INLINE void Matrix4<TYPE>::Camera(const Vector3<TYPE> &pos,const Vector3<TYPE> &look)
{

	Vector3<TYPE> pos2(-pos.x,-pos.y,-pos.z);

	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;	

	_13 = look.x;
	_23 = look.y;
	_33 = look.z;
	_43 = HoeDot(look,pos2);

	const TYPE rvl = (TYPE)sqrtf((look.z*look.z)+(look.x*look.x));

	_11 = look.z / rvl;
	_21 = 0;	
	_31 = -look.x / rvl;
	_41 = pos2.x * _11 + pos2.z * _31;

	_12 = look.y * _31;
	_22 = look.z * _11 - look.x * _31;
	_32 = - look.y * _11;
	_42 = pos2.x * _12 + pos2.y * _22 + pos2.z * _32;

}

template<> HOE_INLINE void Matrix4f::Perspective(const float w,const float h,const float zn,const float zf)
{
	_11 = 2*zn/w;
	_12 = _13 = _14 = 0;
	_22 = 2*zn/h;
	_21 = _23 = _24 = 0;
	_33 = zf/(zf-zn);
	_31 = _32 = 0;
	_34 = 1.0f;
	_43 = zn*zf/(zn-zf);
	_41 = _42 = _44 = 0;

}

template<>
HOE_INLINE void Matrix4v::PerspectiveFov(const vfloat fovX,
										 const vfloat a,
										 const vfloat zn,
										 const vfloat zf)
{
	const vfloat w = (vfloat)1 / tanf(fovX/2);
	const vfloat h = w/a;
	_11 = w;
	_12 = _13 = _14 = 0;
	_22 = h;
	_21 = _23 = _24 = 0;
	_33 = zf/(zf-zn);
	_31 = _32 = 0;
	_34 = 1.0f;
	_43 = zn*zf/(zn-zf);
	_41 = _42 = _44 = 0;

}

template<>
HOE_INLINE void Matrix4v::Ortho(const vfloat w,
								const vfloat h,
								const vfloat zn,
								const vfloat zf)
{
	_11 = (vfloat)2/w;
	_12 = _13 = _14 = 0;
	_22 = (vfloat)2/h;
	_21 = _23 = _24 = 0;
	_33 = (vfloat)1.0f/(zf-zn);
	_31 = _32 = 0;
	_44 = 1.0f;
	_43 = zn/(zn-zf);
	_41 = _42 = _34 = 0;

}

template<>
HOE_INLINE void Matrix4v::Ortho(const vfloat l,
								const vfloat r,
								const vfloat b,
								const vfloat t,
								const vfloat zn,
								const vfloat zf)
{
	_11 = (vfloat)2/(r-l);
	_12 = _13 = _14 = 0;
	_22 = (vfloat)2/(t-b);
	_21 = _23 = _24 = 0;
	_33 = (vfloat)1.0f/(zf-zn);
	_31 = _32 = 0;
	_34 = 0.0f;
	_43 = zn/(zn-zf);
	_41 = (l+r)/(l-r);
	_42 = (t+b)/(b-t);
	_44 = 1.0f;

}

template<>
HOE_INLINE void Matrix4v::Multiply(const Matrix4v &a,const Matrix4v &b)
{

	_11 = b._11 * a._11 + b._21 * a._12 + b._31 * a._13 + b._41 * a._14;
	_12 = b._12 * a._11 + b._22 * a._12 + b._32 * a._13 + b._42 * a._14;
	_13 = b._13 * a._11 + b._23 * a._12 + b._33 * a._13 + b._43 * a._14;
	_14 = b._14 * a._11 + b._24 * a._12 + b._34 * a._13 + b._44 * a._14;

	_21 = b._11 * a._21 + b._21 * a._22 + b._31 * a._23 + b._41 * a._24;
	_22 = b._12 * a._21 + b._22 * a._22 + b._32 * a._23 + b._42 * a._24;
	_23 = b._13 * a._21 + b._23 * a._22 + b._33 * a._23 + b._43 * a._24;
	_24 = b._14 * a._21 + b._24 * a._22 + b._34 * a._23 + b._44 * a._24;

	_31 = b._11 * a._31 + b._21 * a._32 + b._31 * a._33 + b._41 * a._34;
	_32 = b._12 * a._31 + b._22 * a._32 + b._32 * a._33 + b._42 * a._34;
	_33 = b._13 * a._31 + b._23 * a._32 + b._33 * a._33 + b._43 * a._34;
	_34 = b._14 * a._31 + b._24 * a._32 + b._34 * a._33 + b._44 * a._34;

	_41 = b._11 * a._41 + b._21 * a._42 + b._31 * a._43 + b._41 * a._44;
	_42 = b._12 * a._41 + b._22 * a._42 + b._32 * a._43 + b._42 * a._44;
	_43 = b._13 * a._41 + b._23 * a._42 + b._33 * a._43 + b._43 * a._44;
	_44 = b._14 * a._41 + b._24 * a._42 + b._34 * a._43 + b._44 * a._44;

}

template<>
HOE_INLINE void Matrix4v::Multiply(const Matrix4 &m)
{
	Matrix4 a = *this;
	Multiply(a,m);
}

template<>
HOE_INLINE void Matrix4f::MultiplyLeft(const Matrix4 &m)
{
	Matrix4 a = *this;
	Multiply(m,a);
}

template<>
HOE_INLINE void BoundingBox3v::Set(const Vector3v & v)
{
	min = max = v;
	ball = (vfloat)v.MagnitudeF();
}

template<>
HOE_INLINE void BoundingBox3v::Add(const Vector3v & v)
{
	min.Min(v);
	max.Max(v);
	register vfloat m = v.MagnitudeF();
	if (m > ball)
		ball = m;
}	

template<>
HOE_INLINE void BoundingBox3v::Set(const BoundingBox3v & b)
{
	*this = b;
}

template<>
HOE_INLINE void BoundingBox3v::Add(const BoundingBox3v & b)
{
	min.Min(b.min);
	max.Max(b.max);
	if (b.ball > ball)
		ball = b.ball;
}

template<>
HOE_INLINE void BoundingBox3v::Compute(const Vector3v *first, dword numvert, uint stride)
{
	assert(first && numvert > 0 && "bad parameters BoundingBox::Compute");
	Set(*first);
	for (dword i=1; i < numvert;i++)
	{
		register const Vector3v * v = (const Vector3v *)(((const byte*)first)+stride*i);
		Add(*v);
	}
}

//////// func //////////////////////////////////////////

HOE_INLINE Matrix4f* HoeMatrixLookAtLH_(Matrix4f &out,const Vector3f &eye,const Vector3f &at,const Vector3f &up)
{
/*	Vector3f zaxis = at - eye;
	HoeNormal(zaxis);

	Vector3f xaxis;
	HoeCross(up, zaxis, zaxis);
	HoeNormal(xaxis);

	Vector3f yaxis;
	HoeCross(zaxis, xaxis, yaxis);

 xaxis.x           yaxis.x           zaxis.x          0
 xaxis.y           yaxis.y           zaxis.y          0
 xaxis.z           yaxis.z           zaxis.z          0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1*/
 return NULL;
}

HOE_INLINE vfloat HoeDot(const Vector3v &vec1,const Vector3v &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

HOE_INLINE vfloat HoeDot(const Vector2v &vec1,const Vector2v &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

HOE_INLINE void HoeCross(const Vector3v &vec1, const Vector3v &vec2,Vector3v &cross)
{

	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	cross.x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
														
	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	cross.y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
														
	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	cross.z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));
}

HOE_INLINE const Vector3v HoeCross(const Vector3v &vec1, const Vector3v &vec2)
{
	Vector3v cross;
	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	cross.x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
														
	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	cross.y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
														
	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	cross.z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));
	return cross;
}

HOE_INLINE vfloat HoeAngleBetweenVectorsF(const Vector2v &v1, const Vector2v &v2)
{							
	// Get the dot product of the vectors
	vfloat dotProduct = HoeDot(v1, v2);				

	// Get the product of both of the vectors magnitudes
	vfloat vectorsMagnitude = v1.MagnitudeF() * v2.MagnitudeF();

	// Return the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
	return (vfloat)( acosf( dotProduct / vectorsMagnitude ) );
}

HOE_INLINE vdouble HoeAngleBetweenVectors(const Vector3v &v1, const Vector3v &v2)
{							
	// Get the dot product of the vectors
	vdouble dotProduct = HoeDot(v1, v2);				

	// Get the product of both of the vectors magnitudes
	vdouble vectorsMagnitude = v1.Magnitude() * v2.Magnitude();

	// Return the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
	return( acos( dotProduct / vectorsMagnitude ) );
}

// colision

HOE_INLINE bool HoePointInBox(Vector3v &p,
							  vfloat minx,
							  vfloat miny, 
							  vfloat minz, 
							  vfloat maxx, 
							  vfloat maxy, 
							  vfloat maxz)
{
	return !(p.x < minx || p.x > maxx || p.y < miny || p.y > maxy || p.z < minz || p.z > maxz);
}

HOE_INLINE bool HoeSphereInBox(Vector3f &p,float radius,float minx,float miny, float minz, float maxx, float maxy, float maxz)
{
	return !((p.x+radius) < minx || (p.x-radius) > maxx || (p.y+radius) < miny || (p.y-radius) > maxy || (p.z+radius) < minz || (p.z-radius) > maxz);
}

HOE_INLINE void HoePlaneNormal(const Vector3v &a,const Vector3v &b, const Vector3v &c,Vector3v &normal)
{
	HoeCross(b-a,c-a,normal);

	normal.Normalize();
}

HOE_INLINE float HoePlaneDistance(Vector3f &normal, Vector3f &point)
{	
	// Use the Plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.
	// So, we come up with D = -(Ax + By + Cz)
														
	return  - ((normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z));
}

HOE_INLINE float HoeDistance(Vector2f &a, Vector2f &b)
{	
	const register float x = a.x-b.x;
	const register float y = a.y-b.y;
	return  sqrtf(x*x+y*y);
}
HOE_INLINE float HoeDistance(Vector3f &a, Vector3f &b)
{	
	const register float x = a.x-b.x;
	const register float y = a.y-b.y;
	const register float z = a.z-b.z;
	return  sqrtf(x*x+y*y+z*z);
}

HOE_INLINE int HoeClassifySphere(Vector3f &pos, 
				   Vector3f &normal, Vector3f &a, float radius, float &distance)
{
	// Here we use the famous distance formula to find the distance the center point
	// of the sphere is from the triangle's Plane.  
	distance = (normal.x * pos.x + normal.y * pos.y
		+ normal.z * pos.z + HoePlaneDistance(normal, a));

	if(distance >= 0 && distance < radius)
		return P_INTERSECTS;
	else if(distance >= radius)
		return P_FRONT;
	
	return P_BEHIND;
}

bool HoeInsidePolygon(Line2f * lines,int num_lines,const float x,const float y,int i1,int i2)
{
	for (int i=0;i < num_lines;i++)
	{
		if (i == i1 || i == i2)
			continue;
		if (lines[i].Compute(x,y) < 0.00000)
			return false;
	}
	return true;
}


HOE_INLINE bool HoeInsidePolygon(const Vector3v &intersection, const Vector3v &a,const Vector3v &b,const Vector3v &c)
{
	register vdouble angle = 0.0;					
	
	angle += HoeAngleBetweenVectors(a-intersection, b-intersection);
	angle += HoeAngleBetweenVectors(b-intersection, c-intersection);
	angle += HoeAngleBetweenVectors(c-intersection, a-intersection);
											
	return (angle >= vdouble(MATCH_FACTOR * (2.0 * HOE_PI)));
		
}

HOE_INLINE void HoeGetCollisionOffset(Vector3f &normal, float radius, float distance,Vector3f &offset)
{
	offset = Vector3f(0, 0, 0);

	if(distance > 0)
	{
		float distanceOver = radius - distance;
		offset = normal * distanceOver;
	}
	else 
	{
		float distanceOver = radius + distance;
		offset = normal * -distanceOver;
	}
}

HOE_INLINE bool HoeEdgePlanesCollision(Vector3v &center, 
						 Vector3v &a,Vector3v &b,Vector3v &c, Vector3v &normal, vfloat radius)
{
	// Create some variables to hold each edge planes classification with the sphere
	int edge1 = 0, edge2 = 0, edge3 = 0;
	vfloat distance1 = 0, distance2 = 0, distance3 = 0;

	//////////////// Calculate Edge 1 ////////////////

	// Get a vector from the edge of the triangle
	Vector3v vector = b - a;
	Vector3v edgenormal;
	// Get the vector perpendicular from the normal of the triangle and this edge vector
	HoeCross(vector, normal,edgenormal);

	// Before we can call this new vector a normal, we need to normalize it
	edgenormal.Normalize();

	// Find the classification of the sphere and the first Plane
	edge1 = HoeClassifySphere(center, edgenormal, a, radius, distance1);
	if (edge1 == P_BEHIND)
		return false;
	//////////////// Calculate Edge 2 ////////////////

	// Get a vector from the second edge of the triangle
	vector = c - b;

	// Get the vector perpendicular from the normal of the triangle and this edge vector
	HoeCross(vector, normal,edgenormal);

	// Before we can call this new vector a normal, we need to normalize it
	edgenormal.Normalize();

	// Find the classification of the sphere and the first Plane
	edge2 = HoeClassifySphere(center, edgenormal, a, radius, distance2);
	if (edge2 == P_BEHIND)
		return false;
	//////////////// Calculate Edge 3 ////////////////

	// Get a vector from the edge of the triangle
	vector = a - c;

	// Get the vector perpendicular from the normal of the triangle and this edge vector
	HoeCross(vector, normal,edgenormal);

	// Before we can call this new vector a normal, we need to normalize it
	edgenormal.Normalize();

	// Find the classification of the sphere and the first Plane
	edge3 = HoeClassifySphere(center, edgenormal, a, radius, distance3);
	if (edge2 == P_BEHIND)
		return false;
	
	if(edge1 == P_INTERSECTS && fabs(distance1) < radius/2.0f) 
		return true;
	else if(edge2 == P_INTERSECTS && fabs(distance2) < radius/2.0f) 
		return true;
	else if(edge3 == P_INTERSECTS && fabs(distance3) < radius/2.0f) 
		return true;

	return false;
}

#endif // _HE_MATH_INL__
