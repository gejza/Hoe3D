
#include <Max.h>
#include <vector>
#include "MeshBuffer.h"

MeshBuffer::MeshBuffer(bool normals, bool colors, bool tv)
{
	m_exptv = tv;
	m_expc = colors;
	m_expn = normals;
}

DWORD MeshBuffer::AddVertex(const MeshVertex & vert)
{
	// prohledat vsechno, pokud nenalezen tak vlozit
	for (DWORD i=0;i < m_stream.size();i++)
	{
		if (m_stream[i].posIndex != vert.posIndex)
			continue;
		if (m_expc && m_stream[i].colorIndex != vert.colorIndex)
			continue;
		if (m_exptv && m_stream[i].texIndex != vert.texIndex)
			continue;
		if (m_expn && m_stream[i].smoothGroup != vert.smoothGroup)
			continue;
		return i;
	}
	m_stream.push_back(vert);
	return (DWORD)m_stream.size() - 1;
}

bool MeshBuffer::ComputeMeshIndex(INode * node, TimeValue t)
{
	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) {
		return false;
	}
	
	Mesh* mesh = &tri->GetMesh();
	mesh->buildNormals();

	// zkontrolovat
	//if (mesh->getN mesh->getNumVerts()
	if (m_exptv && !mesh->tvFace)
		m_exptv = false;
	if (m_expc && !mesh->vcFace)
		m_expc = false;

	m_stream.clear();
	m_index.clear();
	int vx1, vx2, vx3;
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;

	// jizda po facech
	MeshVertex a,b,c;
	for (int i=0;i < mesh->getNumFaces(); i++)
	{
		a.posIndex = mesh->faces[i].v[vx1];
		b.posIndex = mesh->faces[i].v[vx2];
		c.posIndex = mesh->faces[i].v[vx3];
		if (m_exptv)
		{
			a.texIndex = mesh->tvFace[i].t[vx1];
			b.texIndex = mesh->tvFace[i].t[vx2];
			c.texIndex = mesh->tvFace[i].t[vx3];
		}
		if (m_expc)
		{
			a.colorIndex = mesh->vcFace[i].t[vx1];
			b.colorIndex = mesh->vcFace[i].t[vx2];
			c.colorIndex = mesh->vcFace[i].t[vx3];
		}
		
		a.smoothGroup = b.smoothGroup = c.smoothGroup = mesh->faces[i].getSmGroup();
		m_index.push_back(AddVertex(a));
		m_index.push_back(AddVertex(b));
		m_index.push_back(AddVertex(c));
	}
	// del
	if (needDel) 
	{
		delete tri;
	}
	return true;
}

TriObject* MeshBuffer::GetTriObjectFromNode(INode * node, TimeValue t, int& deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t * GetTicksPerFrame()).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{ 
		TriObject *tri = (TriObject *) obj->ConvertToType(t * GetTicksPerFrame(), 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else 
	{
		return NULL;
	}
}

bool MeshBuffer::ExportIndex(ModelExportFile *file)
{
	file->Printf("\nIndex i_%s\n",m_name);
	// Export the indices
	for (int i=0;i+2 < m_index.size();i+=3)
	{
		file->Printf("\t%d, %d, %d\n",m_index[i],m_index[i+1],m_index[i+2]);
	}
	file->Printf("~Index\n\n");
	return true;
}

bool MeshBuffer::ExportStream(INode * node, TimeValue t, bool local, ModelExportFile * file)
{
	// export streamu
	Matrix3 tm = node->GetObjTMAfterWSM(t * GetTicksPerFrame());
	if (local)
	{
		tm.NoTrans();
	}
	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) 
	{
		return false;
	}
	
	Mesh* mesh = &tri->GetMesh();
	mesh->buildNormals();

	// export stream
	if (m_from == m_to)
		file->Printf("Stream s_%s[xyz",m_name);
	else
		file->Printf("Stream s_%s(%d)[xyz",m_name,(int)t);
	if (m_expn) file->Printf("|normal");
	if (m_expc) file->Printf("|diffuse");
	if (m_exptv) file->Printf("|tex2");
	file->Printf("]\n");

	// vypsat vertexy
	for (int i=0;i < m_stream.size();i++)
	{
		const MeshVertex & v = m_stream[i];
		Point3 pos = tm * mesh->getVert(v.posIndex);
		file->Printf("\t");
		file->Printf("%f", pos.x);
		file->Printf(", ");
		file->Printf("%f", pos.z);
		file->Printf(", ");
		file->Printf("%f", pos.y);
		// normala
		if (m_expn)
		{
			Point3 n = GetVertexNormal(mesh->getRVert(v.posIndex), v.smoothGroup);
			Matrix3 im = tm;
			im.Invert();
			Point3 a1 = im.GetRow(0);
			Point3 a2 = im.GetRow(1);
			Point3 a3 = im.GetRow(2);
			Matrix3 nn;
			nn.Zero();
			nn.SetRow(0, Point3(a1.x, a2.x, a3.x));
			nn.SetRow(1, Point3(a1.y, a2.y, a3.y));
			nn.SetRow(2, Point3(a1.z, a2.z, a3.z));
			n = nn * n;
			file->Printf(";\t");
			file->Printf("%f", n.x);
			file->Printf(", ");
			file->Printf("%f", n.z);
			file->Printf(", ");
			file->Printf("%f", n.y);
		}
		if (m_expc)
		{
			VertColor vc = mesh->vertCol[v.colorIndex];
			DWORD c = 0xff000000;
			c |= (0xff&DWORD(vc.x * 0xff)) << 16;
			c |= (0xff&DWORD(vc.y * 0xff)) << 8;
			c |= (0xff&DWORD(vc.z * 0xff));
			file->Printf("; 0x%x",c);
		}
		if (m_exptv) 
		{
			UVVert tx = mesh->getTVert(v.texIndex);
			file->Printf(";\t ");
			file->Printf("%f", tx.x);
			file->Printf(", ");
			file->Printf("%f", (1.f-tx.y));
		}
		file->Printf("\n");
	}
	// konec

	file->Printf("~Stream // %d,%d,%d\n\n",
		(int)mesh->getNumVerts(),
		(int)m_stream.size(),(int)m_index.size());

	// del
	if (needDel) 
	{
		delete tri;
	}
	return true;
}

Point3 MeshBuffer::GetVertexNormal(RVertex& rv, DWORD smGroup)
{
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv.rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv.rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv.rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv.rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv.ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv.ern[i].getNormal();
				}
			}
		}
	}
	//else {
		// Get the normal from the Face if no smoothing groups are there
	//	vertexNormal = mesh->getFaceNormal(faceNo);
	//}
	
	return vertexNormal;
} 

BOOL MeshBuffer::TMNegParity(Matrix3& m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

bool MeshBuffer::Export(const char * name, INode * node, TimeValue from, TimeValue to, bool local, ModelExportFile * file)
{
	// vypocitat index
	m_name = name;
	m_from = from;
	m_to = to;
	if (!ComputeMeshIndex(node, from))
		return false;

	// export streamu
	for (TimeValue t = m_from;t <= m_to;t++)
	{
		if (!ExportStream(node, t, local, file))
			return false;
	}

	if (!ExportIndex(file))
		return false;
	return true;
}







