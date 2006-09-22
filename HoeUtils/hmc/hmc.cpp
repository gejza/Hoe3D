 
/*#ifndef __STDC__
#define __STDC__ 0
#endif*/

#include "StdAfx.h"
#include "hmc.h"

//#include <fstream>
//#include <iostream>
//using namespace std;
#include "../../Hoe3D/include/hfmt/hmodel_file.h"
#include "../../Hoe3D/include/hfmt/hres_file.h"

bool VertexTest();
bool HoeUtilsTest(); // utils.cpp



bool HMC::Open(const char * name)
{
	line = 0;
	return file.Open(name);
}

const char * HMC::GetNextLine()
{
	line++;
	if (file.ReadLine(line_buffer, sizeof(line_buffer)) == -1)
		return NULL;
	return line_buffer;
}

int HMC::GetActLine()
{
	return line;
}

CBaseStream * HMC::CreateStream(const std::string &name, _FVF &fvf)
{
	CStream * stream = new CStream(name,fvf);
	// tady vcetne indexu
	global.streams.push_back(stream);
	return stream;
}

CBaseIndex * HMC::CreateIndex(const std::string &name)
{
	CIndex * index = new CIndex(name);
	global.indices.push_back(index);
	return index;
}

CBaseMaterial * HMC::CreateMaterial(const std::string &name)
{
	CMaterial * mat = new CMaterial(name);
	global.materials.push_back(mat);
	return mat;
}

ModelShader * HMC::CreateModelShader(const std::string &name)
{
	CModel * model = new CModel(name);
	global.models.push_back(model);
	return model;
}

bool HMC::Finish(void)
{
	unsigned int i;
	for (i=0;i < global.streams.size();i++)
		global.streams[i]->PrintInfo();
	for (i=0;i < global.indices.size();i++)
		global.indices[i]->PrintInfo();
	for (i=0;i < global.materials.size();i++)
		global.materials[i]->PrintInfo();

	for (i=0;i < global.models.size();i++)
		if (global.models[i]->Compile(&global) == false)
			return false;

	return true;
}

bool HMC::Link(HoeUtils::Stream * stream)
{
	hfres_header header = { IDRESHEADER, IDMDLHEADER, IDMDLVER, 0, 0 };
	header.numres = (unsigned int)global.models.size();
	header.numchunks = (unsigned int)global.streams.size() + (unsigned int)global.indices.size() + (unsigned int)global.materials.size();
	stream->Write(&header,sizeof(header));

	if (header.numres <= 0)
		return false;

	// modely
	int namespos = stream->CreateSpace(header.numres * sizeof(hfres_name));
	int chunkpos = stream->CreateSpace(header.numchunks * sizeof(hfres_chunk));

	hfres_name * names = new hfres_name[header.numres];
	memset(names,0, header.numres * sizeof(hfres_name));

	hfres_chunk * chunks = new hfres_chunk[header.numchunks];
	memset(chunks,0, header.numchunks * sizeof(hfres_chunk));

	// export models
	for (size_t im=0;im < global.models.size();im++)
	{
		CModel * model = global.models[im];
		hfres_name * name = &names[im];
		strncpy(name->name,model->GetName().c_str(),sizeof(name->name)-1);
		name->filepos = stream->GetPos();

		name->size = (unsigned int)model->Export(stream);
	}	

	// export streams
	size_t is=0;
	size_t ii=0;

	for (ii=0;ii < global.streams.size();ii++)
	{
		CStream * str = global.streams[ii];
		hfres_chunk * chunk = &chunks[is++];
		chunk->filepos = stream->GetPos();
		chunk->chunkID = str->GetID();
		chunk->size = str->ExportHeader(stream);
		chunk->size += str->ExportData(stream);
	}

	// export indices
	for (ii=0;ii < global.indices.size();ii++)
	{
		CIndex * ind = global.indices[ii];
		hfres_chunk * chunk = &chunks[is++];
		chunk->filepos = stream->GetPos();
		chunk->chunkID = ind->GetID();
		chunk->size = ind->ExportHeader(stream);
		chunk->size += ind->ExportData(stream);
	}

	// export materials
	for (ii=0;ii < global.materials.size();ii++)
	{
		CMaterial * mat = global.materials[ii];
		hfres_chunk * chunk = &chunks[is++];
		chunk->filepos = stream->GetPos();
		chunk->chunkID = mat->GetID();
		chunk->size = mat->ExportHeader(stream);
		chunk->size += mat->ExportData(stream);
	}

	stream->SetPos(namespos);
	stream->Write(names,header.numres * sizeof(hfres_name));

	stream->SetPos(chunkpos);
	stream->Write(chunks,header.numchunks * sizeof(hfres_chunk));

	delete [] names;
	delete [] chunks;
	return true;
}

bool HMC::Link(const char * fileout)
{
	HoeUtils::File file;
	if (file.Open(fileout,HoeUtils::File::mWrite) == false)
		return false;

	return Link(&file);
}

void HMC::Autotracking(const char * name)
{
	printf("Autotracking %s\n", name);
	std::vector<CStream*> s = global.FindStream(name);
	for (int i=0;i < s.size();i++)
	{
		s[i]->Autotracking();
	}
}

