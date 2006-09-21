
#include "HoeMax.h"
#include <vector>
#include "HoeExport.h"
#include "ResourcesDialog.h"


bool HoeMax::ComputeMesh(INode *node, TimeValue t, MeshBuffer & mesh)
{
	int i;
	Mtl* nodeMtl = node->GetMtl();
	Matrix3 tm = node->GetObjTMAfterWSM(t);
	BOOL negScale = TMNegParity(tm);
	int vx1, vx2, vx3;
	
	ObjectState os = node->EvalWorldState(t);

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return false; // Safety net. This shouldn't happen.
	}
	
	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}
	else {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	

	// Export the vertices
	int nVert = obj->getNumVerts(); 
	for (i=0; i<nVert; i++) 
	{
		Point3 v = tm * obj->verts[i];
		point3 vh(v.x,v.z,v.y);
		mesh.pVert.push_back(vh);

		Point3 n = tm * obj->getNormal(i);
		point3 nh(n.x,n.z,n.y);
		nh.Normalize();
		mesh.pvNormal.push_back(nh);
		
	}
	

	for (i=0;i < obj->getNumFaces();i++)
	{
		Point3 n = obj->getFaceNormal(i);
		
		point3 nh(n.x,n.z,n.y);
		mesh.pNormal.push_back(nh);
	}

	int nTVert = obj->getNumTVerts();
	for (i=0;i < nTVert;i++)
	{
		Point3 v = obj->tVerts[i];
		mesh.pTVert.push_back(point2(v.x,1.f-v.y));
	}

	int nCVert = obj->numCVerts;
	for (i=0;i < nCVert;i++)
	{
		Point3 c = obj->vertCol[i];
		point3 ch(c.x,c.y,c.z);
		mesh.pCVert.push_back(ch);
	}

	for (i=0;i < obj->getNumFaces(); i++)
	{
		face f;
		f.pos[0] = obj->faces[i].v[vx1];
		f.pos[1] = obj->faces[i].v[vx2];
		f.pos[2] = obj->faces[i].v[vx3];
		if (nTVert)
		{
			f.tvert[0] = obj->tvFace[i].t[vx1];
			f.tvert[1] = obj->tvFace[i].t[vx2];
			f.tvert[2] = obj->tvFace[i].t[vx3];
		}
		if (nCVert)
		{
			f.cvert[0] = obj->vcFace[i].t[vx1];
			f.cvert[1] = obj->vcFace[i].t[vx2];
			f.cvert[2] = obj->vcFace[i].t[vx3];
		}
		
		mesh.faces.push_back(f);
	}
		
	if (needDel) {
		delete tri;
	}

	return true;
}









