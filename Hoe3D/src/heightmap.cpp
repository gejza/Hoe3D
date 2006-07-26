
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "heightmap.h"
#include "hoe_time.h"

/**
 *	Výšková mapa.
 */
Heightmap::Heightmap(){

	sizeX=0;
	sizeY=0;
	heights=NULL;
}

Heightmap::~Heightmap(){

	SAFE_DELETE_ARRAY(heights);
}

/**
 *	Nahraje vysky do mapy.
 */
int Heightmap::loadHeights(int sizeX, int sizeY, float* f){

	this->sizeX=sizeX;
	this->sizeY=sizeY;
	
	SAFE_DELETE_ARRAY(heights);
	heights=new float[sizeX*sizeY];
	memcpy(heights, f, sizeof(float)*sizeX*sizeY);
	return 1;
}

/**
 * Zmeni velikost vyskove mapy s tim, ze zanecha puvodni vysky.
 * @param sizeX - velikost v ose X (pocet vysek).
 * @param sizeY - velikost v ose Y (pocet vysek).
 */
void Heightmap::resize(int sizeX, int sizeY){

    if(this->sizeX==sizeX && this->sizeY==sizeY) return;
    
    float* newHeights=new float[sizeX*sizeY];

    int pos=0;
    for(int y=0;y<sizeY;y++){
        for(int x=0;x<sizeX;x++){
            
            if(x < this->sizeX && y < this->sizeY) newHeights[pos] = heights[pos];
            else                                   newHeights[pos] = 0.f;
            pos++;
        }
    }
    
    SAFE_DELETE_ARRAY(heights);
    heights=newHeights;
    this->sizeX=sizeX;
    this->sizeY=sizeY;
}

/**
 * Nastavi velikost vyskove mapy. Predchozi vysky budou smazany.
 * @param sizeX - velikost v ose X.
 * @param sizeY - velikost v ose Y.
 */
void Heightmap::setSize(int sizeX, int sizeY){

    this->sizeX = sizeX;
    this->sizeY = sizeY;
    SAFE_DELETE_ARRAY(heights);
    heights=new float[sizeX*sizeY];
}

/**
 * Nastavi velikost vyskove mapy.
 * @param sizeX - velikost v ose X (pocet vysek).
 */
void Heightmap::setSizeX(int sizeX){
    setSize(sizeX, this->sizeY);
}

/**
 * Nastavi velikost vyskove mapy.
 * @param sizeY - velikost v ose Y (pocet vysek).
 */
void Heightmap::setSizeY(int sizeY){
    setSize(this->sizeX, sizeY);
}

/**
 *	Generuje vyskovou mapu.
 *	TODO metody generovani (fraktalni, perlin noise)
 */
void Heightmap::generateMap(){
	
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){

			//height=(abs(x-sizeX/2)*abs(x-sizeX/2) +
			//		abs(y-sizeY/2)*abs(y-sizeY/2))/20.0f;
			
			//if(y%7==0 && x%4==0) height=50.0f;
			//else 
			float height=(float)(10 + sin(3.14/180*x)*x*10 + cos(3.14/180*y*7)*x*2);

			//height=0.0f;
			setHeightAt(x,y,height);
		}
	}	
}

/**
 * Prida sectenim vyskovou mapu.
 * @param heights - vysky k secteni
 * @param x1,y1,x2,y2 - ctverec v teto vyskove mape na kterem se operace aplikuje.
 */
void Heightmap::add(float* heights, int x1, int y1, int x2, int y2){

	combine(heights, x1, y1, x2, y2, ADD);
}

/**
 * Prida odectenim vyskovou mapu.
 * @param heights - vysky k odecteni
 * @param x1,y1,x2,y2 - ctverec v teto vyskove mape na kterem se operace aplikuje.
 */
void Heightmap::sub(float* heights, int x1, int y1, int x2, int y2){

	combine(heights, x1, y1, x2, y2, SUB);
}

/**
 * Prida nasobenim vyskovou mapu.
 * @param heights - vysky k nasobeni
 * @param x1,y1,x2,y2 - ctverec v teto vyskove mape na kterem se operace aplikuje.
 */
void Heightmap::mul(float* heights, int x1, int y1, int x2, int y2){

	combine(heights, x1, y1, x2, y2, MUL);
}

/**
 * Prida delenim vyskovou mapu.
 * @param heights - vysky k deleni
 * @param x1,y1,x2,y2 - ctverec v teto vyskove mape na kterem se operace aplikuje.
 */
void Heightmap::div(float* heights, int x1, int y1, int x2, int y2){

	combine(heights, x1, y1, x2, y2, DIV);
}

/**
 * Zkombinuje vyskovou mapu s jinou operatorem operator.
 */
void Heightmap::combine(float* heights, int x1, int y1, int x2, int y2, int operation){

	int lSizeX=x2-x1;

	if(x1<0) x1=0;
	if(y1<0) y1=0;

	for(int y=y1;y<y2;y++){
		if(y>sizeY) break;

		for(int x=x1;x<x2;x++){
			if(x>sizeX) break;

			float *dest = &this->heights[x + y*sizeX];
			float *src = &heights[x-x1 + (y-y1)*lSizeX];

			switch(operation){
				case ADD:
					*dest += *src;
					break;
				case SUB:
					*dest -= *src;
					break;
				case MUL:
					*dest *= *src;
					break;
				case DIV:
					*dest /= *src;
					break;
			}
		}
	}
}

/**
 *	Vrátí velikost mapy v ose x.
 */
int Heightmap::getSizeX()
{
	return sizeX;
}

/**
 *	Vrátí velikost mapy v ose y.
 */
int Heightmap::getSizeY()
{
	return sizeY;
}

/**
 *	Vrátí výšku v bodì x, y.
 */
float Heightmap::getHeightAt(int x, int y)
{
	return heights[x + y*sizeX];	
}

/**
 *	Vypoèítá normálu z výškové mapy.
 *	(4-okolí)
 */
HoeMath::VECTOR3 Heightmap::getNormalAt(int x, int y)
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

/**
 *	Nastaví výšku v bodì x, y.
 */
void Heightmap::setHeightAt(int x, int y, float height)
{
	heights[x + y*sizeX]=height;
}
