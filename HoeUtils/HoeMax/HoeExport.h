
#ifndef _HOE_EXPORT_CLASSES_
#define _HOE_EXPORT_CLASSES_

struct point3
{
	union
	{
		struct 
		{
			float x,y,z;
		};
		float pole[3];
	};
	point3(float X,float Y,float Z)
	{
		x = X;y = Y;z = Z;
	}
	void operator += (const point3 &p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
	}
	float Magnitude()
	{
		return sqrt((x*x)+(y*y)+(z*z));
	}
	void Normalize()
	{
		float mag = Magnitude();
		if (mag == 0) return;
		x /= mag;
		y /= mag;
		z /= mag;
	}
};

struct point2
{
	union
	{
		struct 
		{
			float x,y;
		};
		float pole[2];
	};
	point2(float X,float Y)
	{
		x = X;
		y = Y;
	}
};

struct face
{
	int pos[3];
	int normal[3];
	int tvert[3];
	int cvert[3];
};


typedef std::vector<point3> point3list; 
typedef std::vector<point2> point2list; 
typedef std::vector<face> faceList;

class MeshBuffer
{
public:
	point3list pVert;
	point3list pNormal;
	point3list pvNormal;
	point2list pTVert;
	point3list pCVert;
	faceList faces;
	
	bool bExportTV;
	bool bExportC;
	bool bComputeN;
	bool bExportN;
	MeshBuffer();
	int GetFace(int index,int start = 0);
	bool TestPoint(int i1,int i2);
	bool Export(const char * name,HoeMax * he);
};

const char * FloatToStr(const float f);


#endif // _HOE_EXPORT_CLASSES_
