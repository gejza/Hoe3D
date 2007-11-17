#include "Surface.h"

/**
 * Surface
 * Jednoduchy povrch z vyskove mapy.
 * TODO provazani s vertex a index listem HoeModelu
 */
Surface::Surface(){

    //TODO zavolat konstruktor HoeModel(1)
    
    createVertexList();
    createIndexList();
    
    vertCounter=0;
    loaded = false;
}

Surface::~Surface() {}


/**
 * Nahraje vysky povrchu.
 */
void Surface::loadHeights(int sizeX, int sizeY, float *f){
	
    heightmap.loadHeights(sizeX, sizeY, f);
    setSize(sizeX, sizeY);
	
	//normal stream
    /*
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
    */
	loaded = true;

}

int Surface::getSizeX(){
    return sizeX;
}

int Surface::getSizeY(){
    return sizeY;
}

float Surface::getDistX(){
	return distX;
}

float Surface::getDistY(){
	return distY;
}

float Surface::getHeightAt(int x, int y){
	return heightMap.getHeightAt(x,y);
}

HoeMath::VECTOR3 Surface::getNormalAt(int x, int y){
	return heightmap.getNormalAt(x,y);
}

/**
 * Zmeni velikost povrchu se zachovanim puvodnich vysek.
 */
void Surface::resize(int sizeX, int sizeY){
	
    heightmap.resize(sizeX, sizeY);
    
    this->sizeX=heightmap.getSizeX();
    this->sizeY=heightmap.getSizeY();
    
    resize();
}

/**
 * Zajisti potrebne pri zmene velikosti terenu.
 */
void Surface::resize(){

    vertCounter=sizeX * sizeY;
    vertexStream.Create(vertCounter, "pnd" , vertCounter*sizeof(Vertex));
    createVertexList();
    createIndexList();
}

/**
 * Nastavi velikost povrchu.
 * @param sizeX - velikost v ose X (pocet vysek)
 * @param sizeY - velikost v ose Y (pocet vysek)
 */
void Surface::setSize(int sizeX, int sizeY){

    this.sizeX=sizeX;
    this.sizeY=sizeY;
    heightmap.setSize(sizeX, sizeY);
    resize();
}

/**
 * Nastavi velikost povrchu.
 * @param sizeX - velikost v ose X (pocet vysek) 
 */
void Surface::setSizeX(int sizeX){
    setSize(sizeX, this->sizeY);
}

/**
 * Nastavi velikost povrchu.
 * @param sizeY - velikost v ose Y (pocet vysek)
 */
void Surface::setSizeY(int sizeY){
    setSize(this->sizeX, sizeY);
}

/**
 * Nastavi vzdalenost mezi body vysek v ose X.
 * @param distX - vzdalenost v ose X.
 */
void Surface::setDistX(float distX){
	this.distX=distX;
}

/**
 * Nastavi vzdalenost mezi body vysek v ose Y.
 * @param distY - vzdalenost v ose Y.
 */
void Surface::setDistY(float distY){
	this.distY=distY;
}

/**
 * Nastavi vzdalenost mezi vertexy vysek.
 * @param distX - vzdalenost v ose X.
 * @param distY - vzdalenost v ose Y.
 */
void Surface::setDist(float distX, float distY){
    this.distX=distX;
    this.distY=distY;
}

/**
 *	Vytvori vertex list (zde vsechny body vyskove mapy).
 */
void Surface::createVertexList(){

	Vertex* v = (Vertex*) vertexStream.Lock();
	
	int vertCounter=0;
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			v[vertCounter].pos.Set(x*distX, heightMap.getHeightAt(x,y), y*distY);
			v[vertCounter].norm = getNormalAt(x,y);
			v[vertCounter].color=0xffffffff;
			
			vertCounter++;
		}
	}

	vertexStream.Unlock();
}

/**
 * Vytvori index list nad vertex listem tohoto povrchu (modelu).
 */
void Surface::createIndexList(){

    indexCounter = 0;
    word* w = (word*) indexStream.Lock();

    int p1, p2, p3, p4;
    for(int y=0; y<sizeY-1; y++){
    for(int x=0; x<sizeX-1; x++){
        
        p1 = x;
        p2 = p1 + 1;
        p3 = p1 + y*sizeX;
        p4 = p3 + 1;

        w[indexCounter  ] = p1;
        w[indexCounter+1] = p2;
        w[indexCounter+2] = p3;
        
        w[indexCounter+3] = p4;
        w[indexCounter+4] = p3;
        w[indexCounter+5] = p2;

        indexCounter+=6;
    }
    }
    
	indexStream.Unlock();
}

/**
 * Vyrenderuje povrch a vrati jeho model.
 */
HoeModel* Surface::render(){

    //HoeMath::MATRIX matWorld;
	//matWorld.Identity();
	//Ref::SetMatrix(matWorld);
	//?GetHoeStates()->SetupMap();
	//Ref::DrawStdObject(&vertexStream, &indexStream, vertCounter, indexCounter);    

	//normaly
	//GetHoeStates()->SetupModel();
	//Ref::DrawLineObject(&normalStream, size*size);
    
    return this;
}