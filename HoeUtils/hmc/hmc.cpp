 
#include "StdAfx.h"
#include "hmc.h"

//#include <fstream>
//#include <iostream>
//using namespace std;
#include "../../Hoe3D/include/hfmt/hmodel_file.h"
#include "../../Hoe3D/include/hfmt/hres_file.h"

int yyparse(void);


int HMC::Compile(const char *fname)
{
	FILE * f = fopen(fname,"rt");
	if (!f)
		return 2;
	HoeSL::Parser::parser_scan(f,this);
	printf("ret = %d\n", yyparse());

	// compile
	fclose(f);

	return 0;
}

int cmp_symbolptr(const void * a, const void * b)
{
	if (reinterpret_cast<const HoeRes::SymbolPointer*>(a)->hash < reinterpret_cast<const HoeRes::SymbolPointer*>(b)->hash)
		return -1;
	if (reinterpret_cast<const HoeRes::SymbolPointer*>(a)->hash > reinterpret_cast<const HoeRes::SymbolPointer*>(b)->hash)
		return 1;
	return 0;
}

int HMC::Link(const char * fileout)
{
	HoeUtils::File file;
	if (file.Open(fileout,HoeUtils::File::mWrite) == false)
		return false;

	// zapsani hlavicek
	int nobj = (int)m_obj.size();

	HoeRes::Head head = { IDHRESHEADER, IDHRESVER, nobj, 'L' };
	file.Write(&head, sizeof(HoeRes::Head));
	if (nobj==0)
		return 0;
	
	HoeRes::SymbolPointer * symbols = new HoeRes::SymbolPointer[nobj];
	memset(symbols, 0, sizeof(HoeRes::SymbolPointer) * nobj);
	unsigned long symb_pos = file.GetPos();
	file.Write(symbols, sizeof(HoeRes::SymbolPointer) * nobj);
	// ulozeni jmen
	for (int i=0;i < nobj;i++)
	{
		std::string n = m_obj[i].name;
		n += "@obj";
		symbols[i].hash = HoeCore::HashString(n.c_str());
		symbols[i].pos = file.GetPos();
		unsigned long sn = n.size();
		file.Write(&sn, sizeof(sn));
		file.Write(n.c_str(), sn);
	}
	// ulozeni objektu
	for (int i=0;i < nobj;i++)
	{
		//symbols[i].
	}	

	qsort(symbols, nobj, sizeof(HoeRes::SymbolPointer), cmp_symbolptr);
	file.SetPos(symb_pos);
	// jeste by chteli seradit....
	file.Write(symbols, sizeof(HoeRes::SymbolPointer) * nobj);

	return 0;
}

HMC::Object * HMC::GetObject(const char * name)
{
	for (size_t i=0;i < m_obj.size();i++)
	{
		if (m_obj[i].name == name)
			return &m_obj.at(i);
	}
	HMC::Object obj;
	obj.name = name;
	obj.pointer = NULL;
	m_obj.push_back(obj);
	return &m_obj.at(m_obj.size()-1);
}

HoeSL::Parser::StreamParser * HMC::BeginStream(const char * name, const char * fvf, bool define)
{
	// vytvorit novou stream
	Object * obj = GetObject(name);
	if (!define)
	{
		// zkontrolovat jestli se jedna o stejny typ
		return NULL;
	}
	if (obj->pointer)
	{
		char err[2048];
		sprintf(err, "Object '%s' already defined.",name);
		Error(err);
		return NULL;
	}
	CStream * stream = new CStream(_FVF(fvf));
	obj->pointer = stream;
	return stream;
}

HoeSL::Parser::IndexParser * HMC::BeginIndex(const char * name, bool define)
{
	// vytvorit novou stream
	Object * obj = GetObject(name);
	if (!define)
	{
		// zkontrolovat jestli se jedna o stejny typ
		return NULL;
	}
	if (obj->pointer)
	{
		char err[2048];
		sprintf(err, "Object '%s' already defined.",name);
		Error(err);
		return NULL;
	}
	CIndex * index = new CIndex();
	obj->pointer = index;
	return index;
}

HoeSL::Parser::MaterialParser * HMC::BeginMaterial(const char * name, bool define)
{
	// vytvorit novou stream
	Object * obj = GetObject(name);
	if (!define)
	{
		// zkontrolovat jestli se jedna o stejny typ
		return NULL;
	}
	if (obj->pointer)
	{
		char err[2048];
		sprintf(err, "Object '%s' already defined.",name);
		Error(err);
		return NULL;
	}
	CMaterial * mat = new CMaterial();
	obj->pointer = mat;
	return mat;
}

HoeSL::Parser::ModelParser * HMC::BeginModel(const char * name, bool define)
{
	// vytvorit novou stream
	Object * obj = GetObject(name);
	if (!define)
	{
		// zkontrolovat jestli se jedna o stejny typ
		return NULL;
	}
	if (obj->pointer)
	{
		char err[2048];
		sprintf(err, "Object '%s' already defined.",name);
		Error(err);
		return NULL;
	}
	CModel * model = new CModel();
	obj->pointer = model;
	return model;
}

#if 0

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

CBasePoint * HMC::CreatePoint(const std::string &name)
{
	CPoint * point = new CPoint(name);
	global.helpers.push_back(point);
	return point;
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
	for (i=0;i < global.helpers.size();i++)
		global.helpers[i]->PrintInfo();

	for (i=0;i < global.models.size();i++)
		if (global.models[i]->Compile(&global) == false)
			return false;

	return true;
}

bool HMC::Link(HoeUtils::Stream * stream)
{
	hfres_header header = { IDRESHEADER, IDMDLHEADER, IDMDLVER, 0, 0 };
	header.numres = (unsigned int)global.models.size();
	header.numchunks = (unsigned int)global.streams.size() 
		+ (unsigned int)global.indices.size() 
		+ (unsigned int)global.materials.size()
		+ (unsigned int)global.helpers.size();
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
	// export helpers
	for (ii=0;ii < global.helpers.size();ii++)
	{
		CBaseHelper * hlp = global.helpers[ii];
		hfres_chunk * chunk = &chunks[is++];
		chunk->filepos = stream->GetPos();
		chunk->chunkID = hlp->GetID();
		chunk->size = hlp->ExportHeader(stream);
		chunk->size += hlp->ExportData(stream);
	}
	stream->SetPos(namespos);
	stream->Write(names,header.numres * sizeof(hfres_name));

	stream->SetPos(chunkpos);
	stream->Write(chunks,header.numchunks * sizeof(hfres_chunk));

	delete [] names;
	delete [] chunks;
	return true;
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

#endif
