#include "Octree.h"

/**
 * Octree
 * Osmistrom pro orezavani vertexu pohledovou kamerou.
 */

Octree::Octree(float sizeX, float sizeY, float sizeZ, int divide){

    setSize(sizeX, sizeY, sizeZ);
    setDivide(divide);
}

Octree::~Octree() {}

/**
 * Nastavi pocet deleni pro kazdou stranu korenove krychle.
 * @param divide - pocet deleni.
 */
void Octree::setDivide(int divide){

    this->divide=divide;
    recalc();
}

/**
 * Nastavi velikost korenove krychle.
 * @param sizeX - velikost v ose X.
 * @param sizeY - velikost v ose Y.
 * @param sizeZ - velikost v ose Z.
 */
void Octree::setSize(float sizeX, float sizeY, float sizeZ){

    this->sizeX=sizeX;
    this->sizeY=sizeY;
    this->sizeZ=sizeZ;
}

/**
 * Prepocita krychle.
 */
void Octree::recalc(){

    //TODO umi SAFE_DELETE_ARRAY vicenasobne pointery?
    SAFE_DELETE_ARRAY(boxes);
    SAFE_DELETE_ARRAY(vertexStreams);

    int nCubes = 2 << divide;
    boxes = new BoundingBox[nCubes][nCubes][nCubes];
    vertexStreamsBox = new HoeStream*[nCubes][nCubes][nCubes];
}

/**
 * Prida vertex stream do octree.
 * @param vertexStream - pridavany vertex stream.
 */
void Octree::addModel(HoeModel* model){

    
    //TODO bud musi byt predem urcen pocet vertex streamu nebo to bude spojovy seznam
    vertexStreams   
}

/**
 * Odebere vertex stream <i>vertexStream</i>.
 * @param vertexStream - odebirany vertex stream.
 */
void Octree::removeStream(HoeStream* vertexStream){

    //TODO odebirani vertex streamu
}

/**
 * Vrati vertex streamy pro kameru <i>cam</i>.
 * @param cam - pohledova kamera.
 */
HoeStream* Octree::getVertexStreams(HoeCamera *cam){

    //TODO prochazeni octree
}