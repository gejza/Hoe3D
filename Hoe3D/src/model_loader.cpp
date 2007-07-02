
#include "StdAfx.h"
#include "utils.h"
#include "ref.h"
#include "filesystem.h"
#include "../include/hfmt/hmodel_file.h"
#include "model_shader.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_model.h"
#include "texture_system.h"
#include "material_system.h"
#include "model_loader.h"
#include <hoe_math.h>
#include "shared.h"
#include <hoe_log.h>


ModelLoader::ModelLoader(HoeLog * log, int flags)
{
	m_log = log;
	m_flags = flags;
	m_reader = HoeFileReader(NULL,0);
}


HoeModel * ModelLoader::LoadModel(const char * name, bool soft)
{
	LOG(m_log)->Log("LoadModel %s",name);

	if (soft)
	{
		LOG(m_log)->Log("Use soft loading.");
		m_soft = true;
	}
	else
		m_soft = false;

	m_reader = GetFileSystem()->FindResource_(name,IDMDLHEADER);
	if (!m_reader) 
	{
		Con_Print("Error find model %s",name);
		return NULL;
	}

	THoeFileDesc desc;
	m_reader.GetFile()->GetDesc(&desc);
	if (desc.version != IDMDLVER)
	{
		Con_Print("Error: '%s'('%s') failed load, bad version %.2f...(%.2f)",name,desc.name,desc.version / 100.f,IDMDLVER / 100.f);
		return NULL;
	}

	Con_Print("Loading model %s from file %s(%.2f)",
		name,
		desc.name,
		desc.version * 0.01f);

	return LoadModel();
}

HoeModel * ModelLoader::LoadModel()
{
	unsigned int ins;

	m_reader.Read(&ins,4);

	int ver = (((ins >> 16) & 0xff) - '0') * 10 + (((ins >> 24) & 0xff) - '0');
	if ((ins & 0xffff) != ('M' | ('S' << 8)) || ver != 10)
	{
		Con_Print(m_log,"Error: bad shader header");
		return NULL;
	}

	m_reader.Read(&ins,4);
	if (ins != hmsInfo)
	{
		Con_Print(m_log, "Error: missing model info");
		return NULL;
	}

	hms_info info;
	m_reader.Read(&info, sizeof(hms_info));

	HoeModel * model = new HoeModel(info.num_streams, info.num_indices, info.num_materials);

	m_reader.Read(&ins,4);

	if (ins == hmsResources)
	{
		LOG(m_log)->Log("reading resources");

		while (1)
		{
			m_reader.Read(&ins,4);
			if (ins == 0)
				break;

			switch ((ins >> 24) & 0xff)
			{
			case HMS_STREAM:
				{
					if (model->GetNumStreams() == (unsigned)info.num_streams)
					{
						Con_Print(m_log,"Error: more streams");
						return NULL;
					}
					LOG(m_log)->Log("resource stream %d",ins & 0xffff);
					HoeStream * stream = GetStream(ins);
					if (!stream)
						return NULL;
					model->AddDefStream(stream);
				}
				break;
			case HMS_INDEX:
				{
					if (model->GetNumIndex() == (unsigned)info.num_indices)
					{
						Con_Print(m_log,"Error: more index");
						return NULL;
					}
					LOG(m_log)->Log("resource index %d",ins & 0xffff);
					HoeIndex * index = GetIndex(ins);
					if (!index)
						return NULL;
					model->AddDefIndex(index);
				}
				break;
			case HMS_MATERIAL:
				{
					if (model->GetNumMats() == (unsigned)info.num_materials)
					{
						Con_Print(m_log,"Error: more materials");
						return NULL;
					}
					LOG(m_log)->Log("resource material %d",ins & 0xffff);
					HoeMaterial * mat = GetMaterial(ins);
					if (!mat)
						return NULL;
					model->AddDefMaterial(mat);
				}
				break;
			default:
				Con_Print(m_log,"Error: unknown resource");
			}

		}
		
		m_reader.Read(&ins,4);
	}
	
	return model;

}

HoeStream * ModelLoader::GetStream(int id)
{
	HoeFileReader reader = m_reader.GetFile()->FindChunk(id);
	if (!reader)
	{
		LOG(m_log)->Log("Error: Resource %x not found",id);
		Con_Print("Error: Resource %x not found",id);
		return NULL;
	}

	hfm_stream header;
	if (!reader.Read(&header, sizeof(hfm_stream)))
	{
		// todo error
		return NULL;
	}

	char fvf[32] = {0};
	if (!reader.Read(fvf, header.size_fvf))
	{
		// todo error
		return NULL;
	}

	LOG(m_log)->Log("Reading stream fvf=%s num=%d size=%d",fvf,header.numvert,header.size);

	HoeStream * stream = new HoeStream(false, m_soft);
	stream->Create(header.numvert,fvf,header.size);
	reader.Read(stream->Lock(),header.size);
	if (m_flags && m_log)
	{
		m_log->Log("Dumping stream-----");
		stream->Dump(m_log);
		m_log->Log("-------------------");
	}
	stream->Unlock();

	return stream;
}

HoeIndex * ModelLoader::GetIndex(int id)
{
	HoeFileReader reader = m_reader.GetFile()->FindChunk(id);
	if (!reader)
	{
		LOG(m_log)->Log("Error: Resource %x not found",id);
		Con_Print("Error: Resource %x not found",id);
		return NULL;
	}

	hfm_index header;
	if (!reader.Read(&header, sizeof(hfm_index)))
	{
		// todo error
		return NULL;
	}

	LOG(m_log)->Log("Reading index num=%d",header.numinx);

	HoeIndex * index = new HoeIndex(m_soft);
	index->Create(header.numinx);
	reader.Read(index->Lock(),header.numinx * 2);
	if (m_flags && m_log)
	{
		m_log->Log("Dumping index-----");
		index->Dump(m_log);
		m_log->Log("-------------------");
	}
	index->Unlock();

	return index;
}

HoeMaterial * ModelLoader::GetMaterial(int id)
{
	HoeFileReader reader = m_reader.GetFile()->FindChunk(id);
	if (!reader)
	{
		LOG(m_log)->Log("Error: Resource %x not found",id);
		Con_Print("Error: Resource %x not found",id);
		return NULL;
	}

	hfm_material header;
	if (!reader.Read(&header, sizeof(hfm_material)))
	{
		// todo error
		return NULL;
	}

	HoeMaterial * mat = new HoeMaterial;

	if (header.texsize)
	{
		char tex[256] = {0};
		reader.Read(tex,header.texsize);
		LOG(m_log)->Log("Reading texture name=%s",tex);
		mat->SetTexture(GetTextureSystem()->GetTexture(tex));
	}
	else
	{
		hfm_color c;
		reader.Read(&c,sizeof(c));
		mat->SetColor(HoeMaterial::Ambient,HoeMaterialColor(c.r,c.g,c.b,c.a));
		reader.Read(&c,sizeof(c));
		mat->SetColor(HoeMaterial::Diffuse,HoeMaterialColor(c.r,c.g,c.b,c.a));
		reader.Read(&c,sizeof(c));
		mat->SetColor(HoeMaterial::Specular,HoeMaterialColor(c.r,c.g,c.b,c.a));
	}


	return mat;
}






