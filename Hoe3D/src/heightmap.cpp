
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "heightmap.h"
#include "texture_system.h"

struct Vec
{
		HoeMath::VECTOR3 pos;
		HoeMath::VECTOR3 norm;
		dword color;
};

struct VecN
{
		HoeMath::VECTOR3 pos;
		dword color;
};

//-------------------------------------------------------
//	vyskova mapa
//-------------------------------------------------------
HoeHeightMap::HoeHeightMap()
{
	sizeX=0;
	sizeY=0;
	heights=NULL;
}

//-------------------------------------------------------
//	destruktor
//-------------------------------------------------------
HoeHeightMap::~HoeHeightMap()
{
	SAFE_DELETE_ARRAY(heights);
}

//-------------------------------------------------------
//	nahravani vyskove mapy
//-------------------------------------------------------
int HoeHeightMap::load(int sizeX, int sizeY, float * f)
{
	sizeX=sizeX;
	sizeY=sizeY;		
	
	SAFE_DELETE_ARRAY(heights);
	heights=new float[sizeX*sizeY];
	memcpy(heights, f, sizeof(float)*sizeX*sizeY);
	return 1;
}

	//-------------------------------------------------------
	//	generator
	//-------------------------------------------------------
void HoeHeightMap::generatePlaneMap(int SizeX, int SizeY, float height)
{
	int x,y;

	sizeX=SizeX;
	sizeY=SizeY;
	
	SAFE_DELETE_ARRAY(heights);
	heights=new float[sizeX*sizeY];

	for(y=0;y<sizeY;y++){
		for(x=0;x<sizeX;x++){

			//height=(abs(x-sizeX/2)*abs(x-sizeX/2) +
			//		abs(y-sizeY/2)*abs(y-sizeY/2))/20.0f;
			
			//if(y%7==0 && x%4==0) height=50.0f;
			//else 
				//height=(float)(10 + sin(3.14/180*x)*x*10 + cos(3.14/180*y*7)*x*2);

			//height=0.0f;
			setHeightAt(x,y,height);
		}
	}
	
}

int HoeHeightMap::getSizeX()
{
	return sizeX;
}

int HoeHeightMap::getSizeY()
{
	return sizeY;
}

float HoeHeightMap::getHeightAt(int x, int y)
{
	return heights[x + y*sizeX];	
}

//-------------------------------------------------------
//	vypocita normalu z vyskove mapy
//	(4-okoli)
//-------------------------------------------------------
HoeMath::VECTOR3 HoeHeightMap::getNormalAt(int x, int y)
{

	HoeMath::VECTOR3 normal(0,0,0);
	float center=getHeightAt(x,y);
	/*float left, right, up, down;

	if(x>0)			left=getHeightAt(x-1,y);	else left=center;
	if(x<sizeX-1)	right=getHeightAt(x+1,y);	else right=center;
	if(y>0)			up=getHeightAt(x,y-1);		else up=center;
	if(y<sizeY-1)	down=getHeightAt(x,y+1);	else down=center;
*/

	// L
	if(x>0)
	{
		float r = center-getHeightAt(x-1,y);
		HoeMath::VECTOR3 n(-r,sqrt(16+r*r),0);
		n.Normalize();
		normal += n;
	}
	if(x<sizeX-1)
	{
		float r = getHeightAt(x+1,y)-center;
		HoeMath::VECTOR3 n(-r,sqrt(16+r*r),0);
		n.Normalize();
		normal += n.Normalize();
	}
	if(y>0)
	{
		float r = center-getHeightAt(x-1,y);
		HoeMath::VECTOR3 n(0,sqrt(16+r*r),r);
		n.Normalize();
		normal += n.Normalize();
	}
	if(y<sizeY-1)
	{
		float r = getHeightAt(x-1,y)-center;
		HoeMath::VECTOR3 n(0,sqrt(16+r*r),r);
		n.Normalize();
		normal += n.Normalize();
	}
	normal.Normalize();

	/*float nx1=center + (left-center)/2.f;
	float nz1=center + (up-center)/2.f;

	float nx2=center + (right-center)/2.f;
	float nz2=center + (down-center)/2.f;
	
	float nx=nx1+nx2;
	float nz=nz1+nz2;
	float length=nx*nx + nz*nz;

	nx/=length;
	nz/=length;

	float ny=1/(nx*nx + nz*nz);
	
	//normal.Set(nx, ny, nz);
	normal.Set(nx, ny, nz);
	normal.Normalize();*/
	
	return normal;
}

void HoeHeightMap::setHeightAt(int x, int y, float height)
{
	heights[x + y*sizeX]=height;
}


//-------------------------------------------------------
//
//	teren
//
//-------------------------------------------------------
HoeQuadTerrain::HoeQuadTerrain() : vertexStream(true), normalStream(true)
{
	loaded = false;
	enabledVertex = NULL;	
}

HoeQuadTerrain::~HoeQuadTerrain()
{
	SAFE_DELETE_ARRAY(enabledVertex);
}

int HoeQuadTerrain::getSize()
{
	return size;
}

float HoeQuadTerrain::getDistX()
{
	return distX;
}

float HoeQuadTerrain::getDistY()
{
	return distY;
}

float HoeQuadTerrain::getHeightAt(int x, int y)
{
	return heightMap.getHeightAt(x,y);
}
	
HoeMath::VECTOR3 HoeQuadTerrain::getNormalAt(int x, int y)
{
	return heightMap.getNormalAt(x,y);
}

//-------------------------------------------------------
//	spocita vzdalenost bodu od usecky
//	bod je v polovine usecky (podle 'x')
//-------------------------------------------------------
float HoeQuadTerrain::getScreenError(float dx, float dy, float y)
{
	float diff;
	
	diff=(float)fabs(y - dy/2.0f);

	if(dy==0.0f) return diff;
									 //	resp. dx*dx
									 //   v
	return (float)fabs(diff - diff*dy/(dy+dx/dy));
	
}

//-------------------------------------------------------
//	funkce pro prevod distance -> LOD
//-------------------------------------------------------
float HoeQuadTerrain::getLOD(float distance)
{
	return (distance-(float)sqrt(distance))/17.0f;
}
	
	//-------------------------------------------------------
	//	settery
	//-------------------------------------------------------
void HoeQuadTerrain::setSizeLevel(int SizeLevel)
{
	sizeLevel=SizeLevel;
	size=(1 << sizeLevel) + 1;
}

void HoeQuadTerrain::setDistX(float DistX)
{
	distX=DistX;
}

void HoeQuadTerrain::setDistY(float DistY)
{
	distY=DistY;
}


	//-------------------------------------------------------
	//	nahravani
	//-------------------------------------------------------
void HoeQuadTerrain::loadHeight(float _distX,float _distY,int _sizeLevel, float *f)
{
		
	//setSizeLevel(_sizeLevel);
	lod=0.f;
	setSizeLevel(6);
	int size=getSize();
	
	distX=_distX;
	distY=_distY;
	distX=4.0f;
	distY=4.0f;
	
	assert(!"doedlat funkci na nahrani mapy");
	//heights.Load(size,size,f);
	//heightMap.generatePlaneMap(size, size);
	load();
}

void HoeQuadTerrain::load()
{
	SAFE_DELETE(enabledVertex);
	enabledVertex=new bool[size*size];
	
	//	maximalni velikosti:
	//	pocet bodu, pocet trojuhelniku
	//	kvuli nutnosti ho predem urcit (meni se jen pri zmene mapy)
	material.SetColor(HoeMaterial::Diffuse,HoeMaterialColor(1,1,1,1));

	int vertNum=size * size;
	vertexStream.Create(vertNum, "pnd" , vertNum*sizeof(Vec));
	createVertexList();
	
	int numTriangles= 2 * (size-0)*(size-0);
	indexStream.Create(numTriangles*3);
	
	//normal stream
	normalStream.Create(size*size*2,"pd", size*size*2*sizeof(VecN));
	VecN * v = (VecN*)normalStream.Lock();
	for (int x=0;x < size;x++)
		for (int y=0;y < size;y++)
		{
			HoeMath::VECTOR3 vec(x*distX,getHeightAt(x,y), y*distY);
			v->pos = vec;
			v->color = 0xffffffff;
			v++;
			v->pos = vec+getNormalAt(x,y)*3;
			v->color = 0xffffffff;
			v++;

		}
	normalStream.Unlock();
	loaded = true;	
}

//-------------------------------------------------------
//	vrati level vertexu
//	0 - nejnizsi vertex level
//-------------------------------------------------------
int HoeQuadTerrain::getVertexLevel(int x, int y)
{
	
	int level = 0;
	int d = 2;
	
	int size=getSize();
	while(d < size){

		if(x % d == d/2){
			if(y % d == 0) return level;
			if(y % d == d / 2) return level + 1;
		}else{
			if(x % d == 0){
				if(y % d == d/2) return level;
			}
		}

		d *= 2;
		level += 2;
	}

	return level;

}

int HoeQuadTerrain::getMaxVertexLevel()
{
	return 2*sizeLevel;
}


bool HoeQuadTerrain::isEnabledVertex(int x, int y)
{
	return enabledVertex[x+size*y];
}

void HoeQuadTerrain::enableVertex(int x, int y)
{
	enabledVertex[x+size*y]=true;
}
void HoeQuadTerrain::disableVertex(int x, int y)
{
	enabledVertex[x+size*y]=false;
}

//-------------------------------------------------------
//	zkontroluje zavislost vertexu (zda muze byt vypnut nebo ne)
//-------------------------------------------------------
bool HoeQuadTerrain::checkVertex(int x, int y)
{
	int level = getVertexLevel(x, y);
	int sx = 1 << ((level + 1) / 2 - 1);
	int sy = 1 << ((level + 1) / 2 - 1);
 
	return checkVertex(x,y,level,sx,sy);

}

bool HoeQuadTerrain::checkVertex(int x, int y, int level, int sx, int sy)
{

	int size=getSize();
	//disableVertex(x,y);

	if(x % 2 == 1 && y % 2 == 0) return true;      // lowest level
	if(x % 2 == 0 && y % 2 == 1) return true;      // lowest level

	if(level % 2 == 1){
				
		if(sy <= y && isEnabledVertex(x,y - sy)) return false;
		if(y + sy < size && isEnabledVertex(x, y + sy)) return false;
		if(sx <= x && isEnabledVertex(x - sx, y)) return false;
		if(x + sx < size && isEnabledVertex(x + sx, y)) return false;
	}
	else{
		
		if(sx <= size){
		  if(sy <= y && isEnabledVertex(x - sx, y - sy)) return false;
		  if(y + sy < size && isEnabledVertex(x - sx, y + sy)) return false;
		}
		if(x + sx < size){
		  if(sy <= y && isEnabledVertex(x + sx, y - sy)) return false;
		  if(y + sy < size && isEnabledVertex(x + sx, y + sy)) return false;
		}

    }

	return true;

}

//-------------------------------------------------------
//	kontrola zavislosti vertexu na vertexech nizsi urovne
//-------------------------------------------------------
void HoeQuadTerrain::checkVertices()
{
	
	int sx,sy,d,even,swap,x,y,size;
	int maxLevel=getMaxVertexLevel();
	for(int level=1;level<maxLevel;level++){
		
		sx = 1 << ((level + 1) / 2 - 1);
		sy = 1 << ((level + 1) / 2 - 1);
		d = 1 << (level / 2 + 1);
 
		even=1 - level % 2;
		swap = 1 - even;
		x = d / 2;
		y = d / 2 * swap;
		size=getSize();

		while(y < size){
			while(x < size){
				if(!isEnabledVertex(x,y) && !checkVertex(x,y,level,sx,sy)){
					enableVertex(x,y);
				}
				x+=d;
			}
			x = d / 2 * swap;
			if(even==1){
				swap = 1 - swap;
				y += d / 2;
			}else{
				y += d;
			}
		}
	}

}


void HoeQuadTerrain::increaseLOD(float increasement){

	lod+=increasement;
}

void HoeQuadTerrain::decreaseLOD(float decreasement){

	lod-=decreasement;
	if(lod<0.f) lod=-1.f;
}
	//-------------------------------------------------------
	//	ulozi do daneho ctverce teren daneho LODu
	//-------------------------------------------------------
void HoeQuadTerrain::setLOD(float _lod)
{
	
	lod=_lod;
	int size=getSize();
	float* lastUp=new float[size];
	float lastLeft;
	float center, right, down;
	float dist1, dist2;
	int x,y;

	float dx1=4*distX*distX;
	float dx2=4*distY*distY;

	for(x=0;x<size;x++)
	{
		lastUp[x]=getHeightAt(x,0);
	}

	//-------------------------------------------------------
	//	oznaceni vertexu k vyrazeni
	//-------------------------------------------------------
	for(y=1;y<size-1;y++)
	{
		
		lastLeft=getHeightAt(0,y);
		for(x=1;x<size-1;x++)
		{
									
			center=	getHeightAt(x,y);
			right=	getHeightAt(x+1,y);
			down=	getHeightAt(x,y+1);
			
			dist1=getScreenError(dx1,(right-lastLeft),center);
			dist2=getScreenError(dx2,(down-lastUp[x]),center);

			//	body, ktere patri do terenu s timto lodem
			if(dist1>lod && dist2>lod){
				
				disableVertex(x,y);
				lastLeft=center;
				lastUp[x]=center;
				
			}else{
				enableVertex(x,y);
			}
		}
			
	}

	checkVertices();

	delete[] lastUp;
	
}

void HoeQuadTerrain::enableAll()
{

	for(int y=0;y<size;y++)
	for(int x=0;x<size;x++)
		enableVertex(x,y);
}

//-------------------------------------------------------
//	vytvoreni vertex listu
//-------------------------------------------------------
void HoeQuadTerrain::createVertexList()
{

	Vec *v=(Vec *)vertexStream.Lock();

	int size=getSize();
	int vertCounter=0;
	for(int y=0;y<size;y++){
		for(int x=0;x<size;x++){
			v[vertCounter].pos.Set(x*distX, heightMap.getHeightAt(x,y), y*distY);
			v[vertCounter].norm = getNormalAt(x,y);
			v[vertCounter].color=0xffffffff;
			
			vertCounter++;
		}
	}

	vertexStream.Unlock();

}

//-------------------------------------------------------
//	vytvoreni index listu
//-------------------------------------------------------
void HoeQuadTerrain::createIndexList()
{
	
	indexCounter=0;
	int size=getSize();
	int size2=size/2;
	
	/*
	createQuadrantList(0,0,size2,size2,size,0);
	createQuadrantList(size,0,size2,size2,size,size);
	createQuadrantList(size,size,size2,size2,0,size);
	createQuadrantList(0,size,size2,size2,0,0);
	*/
	
	int maxVertexLevel=getMaxVertexLevel();

	//if((fw=fopen("tri.txt","wt"))==NULL){
	//	return;
	//}
	//fprintf(fw,"velikost: level %d -> %d\n",sizeLevel, size);
	//fprintf(fw,"max level: %d\n", maxVertexLevel);

	createQuadrantList(0,0,size-1,0,size-1,size-1,maxVertexLevel);
	createQuadrantList(size-1,size-1,0,size-1,0,0,maxVertexLevel);	
	//fprintf(fw,"--------------\n");
	//fclose(fw);
	
}


void HoeQuadTerrain::createQuadrantList(int x0,int y0,int x1,int y1,int x2,int y2,int level)
{

	if(level==0){
		word *w=(word *)indexStream.Lock();		
		w[indexCounter  ]=x0+y0*size;
		w[indexCounter+1]=x2+y2*size;
		w[indexCounter+2]=x1+y1*size;
		

		//fprintf(fw,"%d %d %d\n",w[indexCounter],w[indexCounter+1],w[indexCounter+2]);		
		indexStream.Unlock();
		indexCounter+=3;
		
		return;
	};

	int centerX = (int)(x0 + (x2 - x0) / 2.f);
	int centerY = (int)(y0 + (y2 - y0) / 2.f);

	if(isEnabledVertex(centerX,centerY)){
		createQuadrantList(x0, y0, centerX, centerY, x1, y1, level-1);
		createQuadrantList(x1, y1, centerX, centerY, x2, y2, level-1);
	}else{
		
		word *w=(word *)indexStream.Lock();
		w[indexCounter  ]=x0+y0*size;
		w[indexCounter+1+level%2]=x2+y2*size;
		w[indexCounter+2-level%2]=x1+y1*size;
		

		//fprintf(fw,"%d %d %d\n",w[indexCounter],w[indexCounter+1],w[indexCounter+2]);		
		indexStream.Unlock();
		indexCounter+=3;
		return;
	}

}

//-------------------------------------------------------
//	renderovani
//-------------------------------------------------------
void HoeQuadTerrain::render(HoeCamera *cam)
{
		
	float lod=1.0f;

	if(!loaded) return;
							
	//float distance; //=0.0f;
	
	//lod=LODfunction(distance);	

	setLOD(lod);
	//enableAll();
	createIndexList();

	//printIndexList(&m_index);
	material.Setup();

	int vertCounter=size*size;
	HoeMath::MATRIX matWorld;
	matWorld.Identity();
	Ref::SetMatrix(matWorld);
	GetHoeStates()->SetupMap();
	Ref::DrawStdObject(&vertexStream, &indexStream, vertCounter, indexCounter);

	// zobraz normaly
	//
	GetHoeStates()->SetupModel();
	Ref::DrawLineObject(&normalStream, size*size);
	
}

void HOEAPI HoeQuadTerrain::SetPosCenter(float x, float y, float z)
{
}

void HOEAPI HoeQuadTerrain::SetSize(float sizeX, float sizeY)
{
}

void HOEAPI HoeQuadTerrain::GenerateHeight(int sizeX,int sizeY)
{
}

void HOEAPI HoeQuadTerrain::SetHeightMap(int sizeX,int sizeY, float *f)
{
}

void HOEAPI HoeQuadTerrain::ShowBrush(bool show)
{
}

void HOEAPI HoeQuadTerrain::SetBrush(float x, float y, float radius, dword color)
{
}

void HOEAPI HoeQuadTerrain::MoveHeight(float x, float y, float radius, float value)
{
}


