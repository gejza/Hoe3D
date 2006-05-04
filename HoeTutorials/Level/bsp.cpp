// nahrani bsp

#include "../Common/tutorial.h"
//#include <gl/gl.h>
#include "bsp.h"

const float zoom = 1;

bool LoadBsp(IHoeSceneEnv * env, const char * fname)
{
	HoeGame::HoeFile f;
	f.Open(fname);
	
	BSP_HEADER header;
	//read in header
	f.Read(&header, sizeof(BSP_HEADER));

	//check header data is correct
	if(	header.string[0]!='I' || header.string[1]!='B' ||
		header.string[2]!='S' || header.string[3]!='P' /*||
		header.version  !=0x2E*/ )
	{
		return false;
	}

	// textury
	int numTextures=header.directoryEntries[bspTextures].length/sizeof(BSP_LOAD_TEXTURE);

	//Create space for this many BSP_LOAD_TEXTUREs
	BSP_LOAD_TEXTURE * loadTextures=new BSP_LOAD_TEXTURE[numTextures];
	if(!loadTextures)
	{
		return false;
	}

	//Load textures
	f.Seek(header.directoryEntries[bspTextures].offset);
	f.Read(loadTextures, header.directoryEntries[bspTextures].length);

	IHoeMaterial ** mats = new IHoeMaterial*[numTextures];
	// prehrani do wall
	for (int i=0;i < numTextures;i++)
	{
		const char * p = loadTextures[i].name;
		while (*p) p++;
		p--;
		while (p > loadTextures[i].name && *(p-1) != '/') p--;
		// nahrani

		//w->LoadTexture(i, p);
		mats[i] = env->GetMaterial(p);
	}
	
	delete [] loadTextures;

	//calculate number of vertices
	int numVertices=header.directoryEntries[bspVertices].length/sizeof(BSP_LOAD_VERTEX);
	BSP_LOAD_VERTEX * loadVertices=new BSP_LOAD_VERTEX[numVertices];

	f.Seek(header.directoryEntries[bspVertices].offset);
	f.Read(loadVertices, header.directoryEntries[bspVertices].length);

	//calculate number of load faces
	int numFaces=header.directoryEntries[bspFaces].length/sizeof(BSP_LOAD_FACE);

	//Create space for this many BSP_LOAD_FACES
	BSP_LOAD_FACE * loadFaces=new BSP_LOAD_FACE[numFaces];

	//go to faces in file
	f.Seek(header.directoryEntries[bspFaces].offset);

	//read in the faces
	f.Read(loadFaces, header.directoryEntries[bspFaces].length);

	float * v = new float[numVertices*5];
	for (int i=0;i < numVertices;i++)
	{
		v[i*5] = zoom * loadVertices[i].position.x;
		v[i*5+1] = zoom * loadVertices[i].position.z;
		v[i*5+2] = -zoom * loadVertices[i].position.y;
		v[i*5+3] = loadVertices[i].decalS;
		v[i*5+4] = loadVertices[i].decalT;
	}

	for (int i=0;i < numFaces;i++)
	{
		//loadFaces[i].numVertices++;
		//loadFaces[i].texture;
		// prehazet vertexy
		if (loadFaces[i].type != bspPolygonFace)
			continue;
		
		for (int j=1;j < (loadFaces[i].numVertices+1)/2;j++)
		{
			int i1=loadFaces[i].firstVertexIndex+j;
			int i2=loadFaces[i].firstVertexIndex+loadFaces[i].numVertices-j;

			float tmp[5];
			memcpy(tmp, &v[i1*5], sizeof(float)*5);
			memcpy(&v[i1*5],&v[i2*5], sizeof(float)*5);
			memcpy(&v[i2*5], tmp, sizeof(float)*5);
		}
		//fprintf(stderr,"s:%d\n", loadFaces[i].numVertices);
		IHoeEnv::Poly * p = env->CreatePolygon(loadFaces[i].numVertices);
		

		p->SetPos(&(v[loadFaces[i].firstVertexIndex*5]), sizeof(float)*5);
		p->SetTex(&(v[loadFaces[i].firstVertexIndex*5+3]), sizeof(float)*5);
		p->SetMaterial(mats[loadFaces[i].texture]);
		p->Release();
		

	}
	
	//w->Load(v, ff, numVertices, numFaces);
	
	delete [] mats;
	delete [] v;
	
	delete [] loadVertices;
	delete [] loadFaces;
	
	return true;
}

