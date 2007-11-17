
#include "HoeMax.h"
#include <vector>
#include "HoeExport.h"


int MeshBuffer::GetFace(int index,int start)
{
	for (int i=start;i < faces.size();i++)
	{
		if (faces[i].pos[0] == index)
			return i * 3;
		if (faces[i].pos[1] == index)
			return i * 3 +1;
		if (faces[i].pos[2] == index)
			return i * 3 + 2;
	}
	return -1;
}

bool MeshBuffer::TestPoint(int i1,int i2)
{
	if (faces[i1/3].pos[i1%3] != faces[i2/3].pos[i2%3])
		return false;

	//if (bExportN && faces[i1/3].)

	if (bExportTV && faces[i1/3].tvert[i1%3] != faces[i2/3].tvert[i2%3])
		return false;

	if (bExportC && faces[i1/3].cvert[i1%3] != faces[i2/3].cvert[i2%3])
		return false;

	return true;
}

bool MeshBuffer::Export(const char * name,HoeMax * he)
{
	if (faces.size() == 0) return false;

	int i,j,bod,b,bi,progres = 0;

	if (bExportC && pCVert.size() == 0) 
	{
		//MessageBox(GetActiveWindow(),"warning: Vertex color not defined!",name,MB_OK);
		bExportC = false;
	}
	if (bExportTV && pTVert.size() == 0) 
	{
		//MessageBox(GetActiveWindow(),"warning: Texture UV not defined!",name,MB_OK);
		bExportTV = false;
	}

	/*
	zakladni algorytmus..
	jet podle faceu...
	najit vsechny facy se stejnym cislem..
	zajistit zda maji ... stejnou texturovou poz..
	maji - jeden bod..
	nemaji - vic bodu...
	*/

	int (*mesh)[3] = new int[faces.size()][3];
	for (i=0;i < faces.size();i++)
		for (j=0;j < 3;j++)
			mesh[i][j] = -1;
	int index = 0;
	
	he->Printf("Stream s_%s[xyz",name);
	if (bExportN) he->Printf("|normal");
	if (bExportC) he->Printf("|diffuse");
	if (bExportTV) he->Printf("|tex2");
	he->Printf("]\n");

	for (i=0;i < faces.size();i++)
	{
		// jizda po faces..
		for (j=0;j < 3;j++)
		{
			int proc = int(float(i*3 + j) / float(faces.size() * 3) * 100.f);
			if (proc != progres)
			{
				he->Progress((int)progres);
				progres = proc;
			}

			// jizda po bodech...
			if (mesh[i][j] != -1) continue; // bod je uz ve hre...
			// vsechny facy co obsahujou bod...
			std::vector<int> body;
			bod=-3;
			while ((bod = GetFace(faces[i].pos[j],((bod+3)/3))) != -1)
			{
				body.push_back(bod);
			}
			// moznost spocitat normalu....
			point3 normal(0,0,0);
			if (bComputeN)
			{
				for (b=0;b < body.size();b++)
				{
					normal += pNormal[body[b]/3];
				}
				normal.Normalize();
			}

			// mam body.. ted udelat vertexy..
			for (b = 0;b < body.size();b++)
			{
				bod = body[b];
				if (body[b] == -1)
					continue;
				
				// zapsat bod...
				int ip = faces[bod/3].pos[bod%3];
				he->Printf("\t");
				he->Printf(FloatToStr(pVert[ip].x));
				he->Printf(", ");
				he->Printf(FloatToStr(pVert[ip].y));
				he->Printf(", ");
				he->Printf(FloatToStr(pVert[ip].z));
				

				if (bExportN)
				{
					if (bComputeN)
					{
						he->Printf(";\t");
						he->Printf(FloatToStr(normal.x));
						he->Printf(", ");
						he->Printf(FloatToStr(normal.y));
						he->Printf(", ");
						he->Printf(FloatToStr(normal.z));
					}
					else
					{
						he->Printf(";\t");
						he->Printf(FloatToStr(pvNormal[ip].x));
						he->Printf(", ");
						he->Printf(FloatToStr(pvNormal[ip].y));
						he->Printf(", ");
						he->Printf(FloatToStr(pvNormal[ip].z));
					}

				}


				// priradit k face index..
				mesh[bod/3][bod%3] = index;
				for (bi = b+1;bi < body.size();bi++) // jizda po zbytku
				{
					if (TestPoint(bod,body[bi])) // body sou stejny..
					{
						// na bod face[body[bi]] zapsat index.. 
						mesh[body[bi]/3][body[bi]%3] = index;
						body[bi] = -1;
					}
				}

				index++;
			}
		}
	}
	he->Printf("~Stream // %d,%d,%d\n",pVert.size(),index,faces.size()*3);


	return true;
}

const char * FloatToStr(const float f)
{
	static char buff[256];
	char * p = buff;
	sprintf(buff,"%f",f);
	while (*p)
	{
		if (*p == ',') *p = '.';
		p++;
	}
	return buff;
}

