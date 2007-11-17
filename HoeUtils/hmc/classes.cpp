
#include "StdAfx.h"
#include "classes.h"
#include "../../Hoe3D/include/hfmt/hmodel_file.h"
#include "../../Hoe3D/src/model_shader.h"

using namespace HoeUtils;
using namespace std;

CStream::CStream(_FVF & fvf_) : m_data(fvf_.GetSize()), m_vert(fvf_)
{
}

void CStream::AddVertex(VERTEX & vert)
{
	m_data.AddData(vert.GetData());
}

void CStream::AddNumber(int num)
{
	m_vert.AddNum(num);
	if (m_vert.IsFull())
	{
		AddVertex(m_vert);
		m_vert.Reset();
	}
}

void CStream::AddFloat(float num)
{
	m_vert.AddFloat(num);
	if (m_vert.IsFull())
	{
		AddVertex(m_vert);
		m_vert.Reset();
	}
}
void CStream::AddUnsigned(unsigned long num)
{
	m_vert.AddUnsigned (num);
	if (m_vert.IsFull())
	{
		AddVertex(m_vert);
		m_vert.Reset();
	}
}

unsigned long CStream::ExportHeader(HoeUtils::Stream * stream)
{
	hfm_stream head;
	head.size_fvf = (unsigned long)m_vert.GetString().size();
	head.numvert = m_data.GetNum();
	head.size = m_data.GetSize();
	stream->Write(&head,sizeof(hfm_stream));
	stream->Write(m_vert.GetString().c_str(),head.size_fvf);

	return (sizeof(hfm_stream) + head.size_fvf);
}

unsigned long CStream::ExportData(HoeUtils::Stream * stream)
{
	stream->Write(m_data.GetData(),m_data.GetSize());
	return m_data.GetSize();
}

void CStream::Autotracking()
{
	double px = 0;
	double py = 0;
	double pz = 0;
	if (m_vert.GetString().c_str()[0] != 'p')
	{
		printf("Failed tracking to FVF=%s\n", m_vert.GetString().c_str());
		return;
	}
	for (int i=0; i < m_data.GetNum();i++)
	{
		// scan indexu
		float * v = (float *)m_data.Get(i);
		px += v[0];
		py += v[1];
		pz += v[2];
	}
	px = px / m_data.GetNum();
	py = py / m_data.GetNum();
	pz = pz / m_data.GetNum();
	printf("Move to tracking point (%f,%f,%f)\n", (float)px, (float) py, (float)pz);
	Move((float)-px, 0.f, (float)-pz);
}

void CStream::Move(float x, float y, float z)
{
	if (m_vert.GetString().c_str()[0] != 'p')
	{
		printf("Failed move to FVF=%s\n", m_vert.GetString().c_str());
		return;
	}
	for (int i=0; i < m_data.GetNum();i++)
	{
		// scan indexu
		float * v = (float *)m_data.Get(i);
		v[0] += x;
		v[1] += y;
		v[2] += z;
	}
}

/////////////////////////////////////////////////////////////////
CPoint::CPoint()
{
	x=y=z=0.f;
}

unsigned long CPoint::ExportHeader(HoeUtils::Stream * stream)
{
	hfm_point head;
	memset(head.name,0,sizeof(head.name));
	//strncpy(head.name, name.c_str(), sizeof(head.name)-1);
	head.x = x;
	head.y = y;
	head.z = z;
	stream->Write(&head,sizeof(head));
	return sizeof(head);
}

unsigned long CPoint::ExportData(HoeUtils::Stream * stream)
{
	return 0;
}

/////////////////////////////////////////////////

CIndex::CIndex() : m_data(2)
{
}

void CIndex::AddIndex(unsigned short ind)
{
	m_data.AddData(&ind);
}

unsigned long CIndex::ExportHeader(HoeUtils::Stream * stream)
{
	hfm_index head;
	head.numinx = m_data.GetNum();
	head.type = TRIANGLELIST;
	stream->Write(&head,sizeof(head));
	return sizeof(head);
}

unsigned long CIndex::ExportData(HoeUtils::Stream * stream)
{
	stream->Write(m_data.GetData(),m_data.GetSize());
	return m_data.GetSize();
}

CMaterial::CMaterial()
{
	hastexture = false;
	hasamb = false;
	hasdiff = false;
	hasspec = false;
	alphatest = 0;
}

void CMaterial::SetColor(Par p, CColor * c)
{
	switch (p)
	{
	case MAmbient:
		ambient = *c;
		hasamb = true;
		break;
	case MDiffuse:
		diffuse = *c;
		hasdiff = true;
		break;
	case MSpecular:
		specular = *c;
		hasspec = true;
		break;
	default:
		assert(false && "unknown color parameter");
		break;
	};
}

void CMaterial::SetPar(Par p, int val)
{
	switch (p)
	{
	case MAlphaTest:
		alphatest = val;
		break;
	default:
		assert(false && "unknown material parameter");
		break;
	};
}

void CMaterial::SetTexture(const char * tex)
{
	texture = tex;
	hastexture = true;
}

unsigned long CMaterial::ExportHeader(HoeUtils::Stream * stream)
{
	hfm_material head;
	head.texsize = texture.length();
	head.alphatest = alphatest;
	stream->Write(&head,sizeof(head));
	return sizeof(head);
}

#define COCO(t,f) t.a = f.alpha;t.r = f.red;t.g = f.green;t.b = f.blue;

unsigned long CMaterial::ExportData(HoeUtils::Stream * stream)
{
	if (!texture.empty())
		stream->Write(texture.c_str(),texture.length());
	else
	{
		hfm_color c;
		COCO(c,ambient);
		stream->Write(&c,sizeof(hfm_color));
		COCO(c,diffuse);
		stream->Write(&c,sizeof(hfm_color));
		COCO(c,specular);
		stream->Write(&c,sizeof(hfm_color));
	}
	return texture.length();
}

CModel::CModel()
{
}

void CModel::AddDefStream(const char *name)
{
	//lnStreams.push_back(name);
}

void CModel::AddDefIndex(const char *name)
{
	//lnIndices.push_back(name);

}

void CModel::AddDefMaterial(const char *name)
{
	//lnMaterials.push_back(name);
}

void CModel::AddDefHelper(const char *name)
{
	//lnHelpers.push_back(name);
}

int CModel::Export(HoeUtils::Stream *stream)
{
	return 0;
}

#if 0

bool CModel::Compile(TNamespace * names)
{
	size_t i;

	for (i=0;i < lnStreams.size();i++)
	{
		std::vector<CStream*> s = names->FindStream(lnStreams[i]);
		if (s.empty())
			HoeUtils::fthrow("could not find stream '%s'",lnStreams[i].c_str());
		for (size_t j=0;j < s.size();j++)
			lStreams.push_back(s[j]);
	}
	for (i=0;i < lnIndices.size();i++)
	{
		std::string n = lnIndices[i];
		CIndex * i = names->FindIndex(n); 
		if (i == NULL)
			HoeUtils::fthrow("could not find index '%s'",n.c_str());
		lIndices.push_back(i);
	}
	for (i=0;i < lnMaterials.size();i++)
	{
		CMaterial * m = names->FindMaterial(lnMaterials[i]); 
		if (m == NULL)
			HoeUtils::fthrow("could not find material '%s'",lnMaterials[i].c_str());
		lMaterials.push_back(m);

	}
	for (i=0;i < lnHelpers.size();i++)
	{
		CBaseHelper * m = names->FindHelper(lnHelpers[i]); 
		if (m == NULL)
			HoeUtils::fthrow("could not find helper '%s'",lnHelpers[i].c_str());
		lHelpers.push_back(m);

	}	// helpers

	return true;
}

std::vector<CStream *> TNamespace::FindStream(std::string name)
{
	// tady 
	std::vector<CStream *> s;
	for (int i=0;i < streams.size();i++)
		if (streams[i]->GetName() == name)
			s.push_back(streams[i]);
	return s;
}

CIndex * TNamespace::FindIndex(std::string name)
{
	for (int i=0;i < indices.size();i++)
		if (indices[i]->GetName() == name)
			return indices[i];
	return NULL;
}

CMaterial * TNamespace::FindMaterial(std::string name)
{
	for (int i=0;i < materials.size();i++)
		if (materials[i]->GetName() == name)
			return materials[i];
	return NULL;
}

CBaseHelper * TNamespace::FindHelper(std::string name)
{
	for (int i=0;i < helpers.size();i++)
		if (helpers[i]->GetName() == name)
			return helpers[i];
	return NULL;
}

int CModel::Export(HoeUtils::Stream * stream)
{
	int i;
	int stpos = stream->GetPos();

	unsigned long ver = IDHMSHEADER(1,0);
	stream->Write(&ver,sizeof(unsigned long));

	unsigned long ins = hmsInfo;
	stream->Write(&ins,sizeof(unsigned long));

	hms_info hmsinfo;
	hmsinfo.num_streams = lStreams.size();
	hmsinfo.num_indices = lIndices.size();
	hmsinfo.num_materials = lMaterials.size();
	stream->Write(&hmsinfo,sizeof(hms_info));


	ins = hmsResources;
	stream->Write(&ins,sizeof(unsigned long));
    
	for (i=0;i < lStreams.size();i++)
	{
		ins = lStreams[i]->GetID();
		stream->Write(&ins,sizeof(unsigned long));
		
	}
	for (i=0;i < lIndices.size();i++)
	{
		ins = lIndices[i]->GetID();
		stream->Write(&ins,sizeof(unsigned long));
	}

	for (i=0;i < lMaterials.size();i++)
	{
		ins = lMaterials[i]->GetID();
		stream->Write(&ins,sizeof(unsigned long));
	}
	
	for (i=0;i < lHelpers.size();i++)
	{
		ins = lHelpers[i]->GetID();
		stream->Write(&ins,sizeof(unsigned long));
	}

	ins = 0;
	stream->Write(&ins,sizeof(unsigned long));


	ins = hmsEnd;
	stream->Write(&ins,sizeof(unsigned long));

	return stream->GetPos() - stpos;
}

unsigned long CStream::GetID()
{

	return HMS_GENID(HMS_STREAM,id.GetID());
}

unsigned long CIndex::GetID()
{
	return HMS_GENID(HMS_INDEX,id.GetID());
}

unsigned long CMaterial::GetID()
{
	return HMS_GENID(HMS_MATERIAL,id.GetID());
}

unsigned long CPoint::GetID()
{
	return HMS_GENID(HMS_POINT,id.GetID());
}

#endif
