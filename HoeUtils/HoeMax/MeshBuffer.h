
#ifndef _HOE_EXPORT_MESH_
#define _HOE_EXPORT_MESH_

#include "ExportFile.h"

struct MeshVertex
{
	int posIndex;
	int colorIndex;
	int texIndex;
	DWORD smoothGroup;
};

typedef std::vector<MeshVertex> VertexList;
typedef std::vector<DWORD> IndexList;

class MeshBuffer
{
protected:
	VertexList m_stream;
	IndexList m_index;
	bool m_exptv;
	bool m_expc;
	bool m_expn;
	TimeValue m_from;
	TimeValue m_to;
	const char * m_name;

	DWORD AddVertex(const MeshVertex & vert);
	bool ComputeMeshIndex(INode * node, TimeValue t);
	bool ExportIndex(ModelExportFile * file);
	bool ExportStream(INode * node, TimeValue t, bool local, ModelExportFile * file);
	TriObject* GetTriObjectFromNode(INode * node, TimeValue t, int& deleteIt);
	BOOL	TMNegParity(Matrix3& m);
	Point3 GetVertexNormal(RVertex& rv, DWORD smGroup);
public:
	MeshBuffer(bool normals, bool colors, bool tv);
	bool Export(const char * name, INode * node, TimeValue from, TimeValue to, bool local, ModelExportFile * file);
};

#endif // _HOE_EXPORT_MESH_

