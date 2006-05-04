
#include "HoeMax.h"
#include <vector>
#include "HoeExport.h"
#include "ResourcesDialog.h"

/****************************************************************************

  Global output [Scene info]
  
****************************************************************************/


// Dump some global animation information.
void HoeMax::ExportGlobalInfo()
{
	struct tm *newtime;
	time_t aclock;

	time( &aclock );
	newtime = localtime(&aclock);

	TSTR today = _tasctime(newtime);	// The date string has a \n appended.
	today.remove(today.length()-1);		// Remove the \n

	// Start with a file identifier and format version
	fprintf(pStream, "// %s\t%.2f\n", this->LongDesc(), this->Version()*0.01f);
	fprintf(pStream, "// %s\t%s\n", this->AuthorName(), this->CopyrightMessage());
	fprintf(pStream, "// \"%s\" - %s\n", FixupName(ip->GetCurFileName()), (TCHAR*)today);
	fprintf(pStream, "\n");

}

void HoeMax::ExportMesh(MeshItem *mesh)
{
	char progressmsg[512];
	sprintf(progressmsg,"Export node '%s' (%d/%d)",mesh->node->GetName(),nCurNode,nTotalNodeCount);
	ip->ProgressStart(progressmsg, TRUE, fn, NULL);
	nCurNode++;

	MeshBuffer stream;
	
	stream.bExportN = stream.bExportTV = true;
	stream.bExportC = false;

	if (!ComputeMesh(mesh->node,0,stream))
		return;	

	stream.Export(FixupName(mesh->node->GetName()),this);
}

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
	
	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) {
		return false;
	}
	
	Mesh* obj = &tri->GetMesh();
	obj->buildNormals();


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

void HoeMax::DumpMaterial(Mtl* mtl)
{

	TimeValue t = 0;//GetStaticFrame();
	
	if (!mtl) return;

	TSTR className;
	mtl->GetClassName(className);
	fprintf(pStream,"// Mat: %s\n",FixupName(className));

	fprintf(pStream,"Material %s\n",FixupName(mtl->GetName()));

	// We know the Standard material, so we can get some extra info
	//if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
	//	StdMat* std = (StdMat*)mtl;

		fprintf(pStream,"\tDiffuse = %s\n", Format(mtl->GetDiffuse(t)));
		fprintf(pStream,"\tAmbient = %s\n", Format(mtl->GetAmbient(t)));
		fprintf(pStream,"\tSpecular = %s\n", Format(mtl->GetSpecular(t)));


	fprintf(pStream,"~Material\n\n");
}


BOOL HoeMax::TMNegParity(Matrix3& m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

TriObject* HoeMax::GetTriObjectFromNode(INode * node, TimeValue t, int& deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}


/****************************************************************************

  Misc Utility functions
  
****************************************************************************/

// Return an indentation string
TSTR HoeMax::GetIndent(int indentLevel)
{
	TSTR indentString = "";
	for (int i=0; i<indentLevel; i++) {
		indentString += "\t";
	}
	
	return indentString;
}

Point3 HoeMax::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
} 


/****************************************************************************

  String manipulation functions
  
****************************************************************************/

#define CTL_CHARS  31
#define SINGLE_QUOTE 39

// Replace some characters we don't care for.
TCHAR* HoeMax::FixupName(TCHAR* name)
{
	static char buffer[256];
	TCHAR* cPtr;
	
	if (*name >= '0' && *name <= '9')
	{
		*buffer = '_';
		_tcscpy(buffer + 1, name);
	}
	else
	{
		_tcscpy(buffer, name);
	}
	
	cPtr = buffer;
	
    while(*cPtr) {
		if (*cPtr == ' ')
			*cPtr = '_';
		else if (*cPtr == '"')
			*cPtr = SINGLE_QUOTE;
        else if (*cPtr <= CTL_CHARS)
			*cPtr = _T('_');
        cPtr++;
    }
	
	return buffer;
}

// International settings in Windows could cause a number to be written
// with a "," instead of a ".".
// To compensate for this we need to convert all , to . in order to make the
// format consistent.
void HoeMax::CommaScan(TCHAR* buf)
{
    for(; *buf; buf++) if (*buf == ',') *buf = '.';
}

TSTR HoeMax::Format(int value)
{
	TCHAR buf[50];
	
	sprintf(buf, _T("%d"), value);
	return buf;
}


TSTR HoeMax::Format(float value)
{
	TCHAR buf[40];
	
	sprintf(buf, "%f", value);
	CommaScan(buf);
	return TSTR(buf);
}

TSTR HoeMax::Format(Point3 value)
{
	TCHAR buf[120];
	TCHAR fmt[120];
	
	sprintf(fmt, "%s\t%s\t%s", szFmtStr, szFmtStr, szFmtStr);
	sprintf(buf, fmt, value.x, value.y, value.z);

	CommaScan(buf);
	return buf;
}

TSTR HoeMax::Format(Color value)
{
	TCHAR buf[160];
	//TCHAR fmt[160];
	
	//sprintf(fmt, "%s\t%s\t%s\t%s", szFmtStr, szFmtStr, szFmtStr, szFmtStr);
	sprintf(buf,"{ %f, %f, %f}", value.r, value.g, value.b);

	//CommaScan(buf);
	return buf;

}

TSTR HoeMax::Format(AngAxis value)
{
	TCHAR buf[160];
	TCHAR fmt[160];
	
	sprintf(fmt, "%s\t%s\t%s\t%s", szFmtStr, szFmtStr, szFmtStr, szFmtStr);
	sprintf(buf, fmt, value.axis.x, value.axis.y, value.axis.z, value.angle);

	CommaScan(buf);
	return buf;
}


TSTR HoeMax::Format(Quat value)
{
	// A Quat is converted to an AngAxis before output.
	
	Point3 axis;
	float angle;
	AngAxisFromQ(value, &angle, axis);
	
	return Format(AngAxis(axis, angle));
}

TSTR HoeMax::Format(ScaleValue value)
{
	TCHAR buf[280];
	
	sprintf(buf, "%s %s", Format(value.s), Format(value.q));
	CommaScan(buf);
	return buf;
}

