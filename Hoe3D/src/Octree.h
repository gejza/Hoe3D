#pragma once

#include "hoe3d_math.h"

class Octree{

    private:
        struct root{
            BoundingBox* box;
            root* nodes[8];
        } Root;
        
        //pole krychli a vertexu. indexy si odpovidaji.
        BoundingBox*** boxes;
        HoeStream**** vertexStreamsBox;
        
        //pole modelu tohoto octree
        HoeModel* models;
        
        //pocatek korenove krychle
        //HoeMath:VECTOR3 start;
        //float sizeX, sizeY, sizeZ;

        // omezeni
        //velikost nejmensi krychle
        float minSizeX, minSizeY, minSizeZ;
        //pocet deleni
        int divide;
        
    public:
        Octree(void);
        ~Octree(void);
        
        void setSize(float sizeX, float sizeY, float sizeZ);
        void setDivide(int divide);
        
        void addStream(HoeStream* vertexStream);
        void removeStream(HoeStream* vertexStream);
        
        HoeStream* getVertexStreams(HoeCamera* cam);
};
