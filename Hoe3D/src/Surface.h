#ifndef _Surface_H_
#define _Surface_H_

#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"

class Surface : HoeModel{

    private:
        struct Vertex{
	        HoeMath::VECTOR3 pos;
	        HoeMath::VECTOR3 norm;
	        dword color;
        };
        
        HoeHeightMap heightmap;

        int vertCounter, indexCounter;
        bool loaded;
        //HoeStream vertexStream;
	    //HoeIndex indexStream;
	    //HoeStream normalStream;

    protected:

        virtual void createVertexList();
        virtual void createIndexList();

    public:
	    Surface();
	    ~Surface();

	    HoeMath::VECTOR3 getNormalAt(int x, int y);
	    float getDistX();
	    float getDistY();
	    virtual int getSizeX();
        virtual int getSizeY();
    	
	    void setDistX(float DistX);
	    void setDistY(float DistY);
	    virtual void setSizeX(int sizeX);
        virtual void setSizeY(int sizeY);	
    	
	    virtual void loadHeight(float _distX,float _distY,int _sizeLevel, float *);
        virtual void loadHeight(Heightmap* heightmap);

	    //virtual void Create(float width, float height);
	    virtual HoeModel* render();	    
}

#endif