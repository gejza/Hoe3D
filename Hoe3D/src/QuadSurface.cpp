#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "heightmap.h"
#include "hoe_time.h"



/*
struct VertexN
{
		HoeMath::VECTOR3 pos;
		dword color;
};
*/

/**
 *  QuadSurface
 *	Povrch implementovaný pomocí Quad závislostí pro triangulaci.
 */

QuadSurface::QuadSurface(){	

	enabledVertex = NULL;
}

QuadSurface::~QuadSurface(){

	SAFE_DELETE_ARRAY(enabledVertex);
}

//-------------------------------------------------------
//	spocita chybu pri nahrazeni vysky v povrchu jeho 
//      interpolacni hodnotou vzhledem k jeho sousedum 
//      a vzhledem ke kamere
//	body vysek jsou ekvidistantni
//-------------------------------------------------------
/*
float QuadSurface::getScreenError(HoeCamera* cam, float height1, float height2, float height3, float dist)
{
	float heighti, diff;
	
	heighti=height1 + (height3-height1)*.5f;

	diff=height2-heighti;
	if(diff<0) diff*=-1.f;
	
	odchylka = cam->projekcniMatice * [0 0 diff] * (koef*dist);

	return odchylka;
	
}
*/

//-------------------------------------------------------
//	funkce pro prevod distance -> LOD
//-------------------------------------------------------
/*
float QuadSurface::getLOD(float distance)
{
	return (distance-(float)sqrt(distance))/17.0f;
}
*/
	
	//-------------------------------------------------------
	//	settery
	//-------------------------------------------------------
void QuadSurface::setSizeLevel(int SizeLevel)
{
	sizeLevel=SizeLevel;
	size=(1 << sizeLevel) + 1;

    SAFE_DELETE(enabledVertex);
	enabledVertex=new bool[size*size];
}

//-------------------------------------------------------
//	vrati level vertexu
//	0 - nejnizsi vertex level
//-------------------------------------------------------

int QuadSurface::getVertexLevel(int x, int y){
	
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

int QuadSurface::getMaxVertexLevel(){
	return 2*sizeLevel;
}


bool QuadSurface::isEnabledVertex(int x, int y){
	return enabledVertex[x+size*y];
}

void QuadSurface::enableVertex(int x, int y){
	enabledVertex[x+size*y]=true;
}
void QuadSurface::disableVertex(int x, int y){
	enabledVertex[x+size*y]=false;
}

//-------------------------------------------------------
//	zkontroluje zavislost vertexu (zda muze byt vypnut nebo ne)
//-------------------------------------------------------
bool QuadSurface::checkVertex(int x, int y){

	int level = getVertexLevel(x, y);
	int sx = 1 << ((level + 1) / 2 - 1);
	int sy = 1 << ((level + 1) / 2 - 1);
 
	return checkVertex(x, y, level, sx, sy);

}

bool QuadSurface::checkVertex(int x, int y, int level, int sx, int sy){

	int size=getSize();
	//disableVertex(x,y);

	if(x % 2 == 1 && y % 2 == 0) return true;      // lowest level
	if(x % 2 == 0 && y % 2 == 1) return true;      // lowest level

	if(level % 2 == 1){
		
		if(sy <= y && isEnabledVertex(x, y - sy)) return false;
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
void QuadSurface::checkVertices(){
	
	int sx, sy, d, even, swap, x, y, size;
	int maxLevel = getMaxVertexLevel();
	for(int level=1; level<maxLevel; level++){
		
		sx = 1 << ((level + 1) / 2 - 1);
		sy = 1 << ((level + 1) / 2 - 1);
		d = 1 << (level / 2 + 1);
 
		even = 1 - level % 2;
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

/**
 * Nastavi LOD pro povrch s maximalni chybou(odchylkou) obrazu <i>maxScreenError</i>.
 * @param maxScreenError - maximalni odchylka obrazu od skutecnosti.
 */
void QuadSurface::setLOD(const float maxScreenError){

	static TimeMeter tmSetting("setting lod ", true);
	static TimeMeter tmCheck("checkVertices()", true);

	int size=getSize();
	float* lastUp=new float[size];
	float lastLeft;
	float center, right, down;
	float dist1, dist2;
	int x,y;

	tmSetting.Begin();

    //prvni radka hornich bodu
	for(x=0;x<size;x++){
		lastUp[x] = getHeightAt(x,0);
	}
	
	//oznaceni vertexu k vyrazeni	
	for(y=1;y<size-1;y++){
		
		lastLeft=getHeightAt(0,y);
		for(x=1;x<size-1;x++){
									
			center=	getHeightAt(x,y);
			right=	getHeightAt(x+1,y);
			down=	getHeightAt(x,y+1);
			
			//docasne - vsechny body
			dist1=maxScreenError+1;//getScreenError(lastLeft, center, right);
			dist2=maxScreenError+1;//getScreenError(lastUp[x], center, down);

			//body, ktere patri do terenu s timto lodem
			if(dist1>maxScreenError && dist2>maxScreenError){
				
				disableVertex(x,y);
				lastLeft=center;
				lastUp[x]=center;
				
			}else{
				enableVertex(x,y);
			}
		}
			
	}
	tmSetting.End();

	tmCheck.Begin();
	checkVertices();
	tmCheck.End();

	delete[] lastUp;
	
}

void QuadSurface::enableAll(){

	for(int y=0;y<size;y++)
	for(int x=0;x<size;x++)
		enableVertex(x,y);
}

//-------------------------------------------------------
//	vytvoreni index listu
//-------------------------------------------------------
void QuadSurface::createIndexList(){
	
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


void QuadSurface::createQuadrantList(int x0,int y0,int x1,int y1,int x2,int y2,int level){

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
void QuadSurface::render(HoeCamera *cam){

	static TimeMeter tm("teren", true);
	static TimeMeter tmLOD("setLOD()", true);
	static TimeMeter tmIndexList("createIndexList()", true);
	static TimeMeter tmDraw("drawing ", true);
	tm.Begin();
	//float lod=1.0f;

	//if(!loaded) return;
							
	//float distance; //=0.0f;
	
	lod=(float)cam->GetMapPos().xyz.Magnitude();

	tmLOD.Begin();
	setLOD(lod);
	tmLOD.End();
	//enableAll();
	tmIndexList.Begin();
	createIndexList();
	tmIndexList.End();
	//printIndexList(&m_index);

	tmDraw.Begin();
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
	tmDraw.End();
	tm.End();
}

/*
HeightMapSurfaceSet::HeightMapSurfaceSet(QuadSurface * ter)
{
	m_ter = ter;
}

void HeightMapSurfaceSet::LoadHeight(float _distX,float _distY,int _sizeLevel, float *f)
{
	assert(m_ter);
	m_ter->loadHeight(_distX, _distY, _sizeLevel, f);
}
*/