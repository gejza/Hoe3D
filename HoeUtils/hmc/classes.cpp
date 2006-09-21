
#include "StdAfx.h"
#include "classes.h"
#include "parser.h"
#include "../../Hoe3D/include/hfmt/hmodel_file.h"
#include "../../Hoe3D/src/model_shader.h"

using namespace HoeUtils;
using namespace std;

CStream::CStream(const std::string &name_, _FVF & fvf_) : data(fvf_.GetSize())
{
	name = name_;
	fvf = fvf_;
}

void CStream::AddVertex(VERTEX & vert)
{
	data.AddData(vert.GetData());
}

// vypise informace o objektu na stdout
void CStream::PrintInfo(void)
{
	printf("Stream \"%s\" pocet vertexu: %d(%d) o fvf=%s(%d)\n",name.c_str(),data.GetNum(),data.GetMax(),fvf.GetString().c_str(),fvf.GetSize());
}

unsigned long CStream::ExportHeader(HoeUtils::Stream * stream)
{
	hfm_stream head;
	head.size_fvf = (unsigned long)fvf.GetString().size();
	head.numvert = data.GetNum();
	head.size = data.GetSize();
	stream->Write(&head,sizeof(hfm_stream));
	stream->Write(fvf.GetString().c_str(),head.size_fvf);

	return (sizeof(hfm_stream) + head.size_fvf);
}

unsigned long CStream::ExportData(HoeUtils::Stream * stream)
{
	stream->Write(data.GetData(),data.GetSize());
	return data.GetSize();
}

CIndex::CIndex(const std::string &name_) : data(2)
{
	name = name_;
}

void CIndex::AddIndex(unsigned short ind)
{
	data.AddData(&ind);
}

// vypise info na stdout
void CIndex::PrintInfo(void)
{
	printf("Index \"%s\" pocet indexu: %d(%d)\n",name.c_str(),data.GetNum(),data.GetMax());
}

unsigned long CIndex::ExportHeader(HoeUtils::Stream * stream)
{
	hfm_index head;
	head.numinx = data.GetNum();
	head.type = TRIANGLELIST;
	stream->Write(&head,sizeof(head));
	return sizeof(head);
}

unsigned long CIndex::ExportData(HoeUtils::Stream * stream)
{
	stream->Write(data.GetData(),data.GetSize());
	return data.GetSize();
}

CMaterial::CMaterial(const std::string &name_)
{
	name = name_;
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

void CMaterial::PrintInfo(void)
{
	printf("Material \"%s\" tex: %s\n",name.c_str(),texture.c_str());
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

CModel::CModel(const std::string &name_)
{
	name = name_;
}

void CModel::AddDefStream(const char *name)
{
	lnStreams.push_back(name);
}

void CModel::AddDefIndex(const char *name)
{
	lnIndices.push_back(name);

}

void CModel::AddDefMaterial(const char *name)
{
	lnMaterials.push_back(name);
}

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

