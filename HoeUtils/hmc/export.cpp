
#include "StdAfx.h"
#include "classes.h"
#include "../../Hoe3D/include/hfmt/hmodel_file.h"

using namespace HoeUtils;

int Object::Export(HoeUtils::File * file)
{
	return 0;
}

bool MainBuffer::Export(const char* name)
{
	File file;
	unsigned int i,j;

	hfm_header head = {IDMDLHEADER,IDMDLVER,0};  

	for (i = 0;i < this->m_objs.size();i++)
	{
		if (m_objs[i]->GetType() == Model)
		{
			head.numtext++;
		}
	}

	if (!head.numtext)
	{
		printf("No model to export\n");
		return false;
	}

	if (!file.Open(name,File::mWrite))
		return false;

	file.Write(&head,sizeof(hfm_header));

	hfm_name * names = new hfm_name[head.numtext];
	memset(names,0,sizeof(hfm_name) * head.numtext);
	file.Write(names,sizeof(hfm_name) * head.numtext);

	j = 0;
	for (i = 0;i < this->m_objs.size();i++)
	{
		if (m_objs[i]->GetType() == Model)
		{
			strcpy(names[j].name,m_objs[i]->m_name.c_str());
			names[j].filepos = reinterpret_cast<ModelBuffer*>(m_objs[i])->Export(&file);
			j++;
		}
	}

	file.SetPos(sizeof(hfm_header));
	file.Write(names,sizeof(hfm_name) * head.numtext);

	file.Close();
	return true;
}

int StreamBuffer::Export(HoeUtils::File * file)
{
	if (m_fpos != -1)
		return m_fpos;

	m_fpos = file->GetPos();

	// export stream header
	hfm_stream head;
	head.size_fvf = strlen(this->m_fvf.GetString());
	head.size = m_data->GetSize();
	head.numvert = m_data->GetNum();
	file->Write(&head,sizeof(head));
	if (head.size_fvf)
		file->Write(m_fvf.GetString(),head.size_fvf);
	file->Write(this->m_data->GetData(),head.size);
	return m_fpos;
}

int IndexBuffer::Export(HoeUtils::File * file)
{
	if (m_fpos != -1)
		return m_fpos;

	m_fpos = file->GetPos();

    // export index header
	hfm_index head;
	head.type = 0;
	head.numinx = this->m_data.GetNum();

	file->Write(&head,sizeof(head));
	file->Write(this->m_data.GetData(),m_data.GetSize());
	return m_fpos;
}

int Material::Export(HoeUtils::File * file)
{
	if (m_fpos != -1)
		return m_fpos;

	m_fpos = file->GetPos();

	if (flags & HMAT_AMBIENT)
        printf("%s:ambient %f %f %f %f\n",m_name.c_str(),ambient.a,ambient.r,ambient.g,ambient.b);
	if (flags & HMAT_DIFFUSE)
        printf("%s:diffuse %f %f %f %f\n",m_name.c_str(),diffuse.a,diffuse.r,diffuse.g,diffuse.b);
	if (flags & HMAT_TEXTURE)
        printf("%s:tex %s\n",m_name.c_str(),texname.c_str());

	hfm_material mat;
	mat.params = this->flags;

	if (flags & HMAT_AMBIENT)
	{
	}

	if (flags & HMAT_DIFFUSE)
	{
	}
	if (flags & HMAT_TEXTURE)
	{
	}
    // export index header
	//hfm_index head;
	//head.type = 0;
	//head.numinx = this->m_data.GetNum();

	//file->Write(&head,sizeof(head));
	//file->Write(this->m_data.GetData(),m_data.GetSize());
	return m_fpos;
}

int ModelBuffer::Export(HoeUtils::File * file)
{
	unsigned int i;

	if (m_fpos != -1)
		return m_fpos;

	int nStreams = 0;
	int nIndex = 0;

	for (i = 0;i < this->m_objs.size();i++)
	{
		m_objs[i]->Export(file);
		switch (m_objs[i]->GetType())
		{
		case Stream:
			nStreams++;break;
		case Index:
			nIndex++;break;
		}
	}


	m_fpos = file->GetPos();

	// save headers
	hfm_chunk modelchunk = { CHUNKID_MODEL,0};
	file->Write(&modelchunk,sizeof(hfm_chunk));

	// export stream list
	hfm_chunk strlist = { CHUNKID_STREAMLIST, nStreams * sizeof(int)};
	modelchunk.size += sizeof(hfm_chunk) + strlist.size;
	file->Write(&strlist,sizeof(hfm_chunk));
	for (i = 0;i < this->m_objs.size();i++)
	{
		if (m_objs[i]->GetType() == Stream)
		{
			int p = m_objs[i]->Export(file);
			file->Write(&p,sizeof(int));
		}
	}

	// export index list
	hfm_chunk indexlist = { CHUNKID_INDEXLIST, nIndex * sizeof(int)};
	modelchunk.size += sizeof(hfm_chunk) + indexlist.size;
	file->Write(&indexlist,sizeof(hfm_chunk));
	for (i = 0;i < this->m_objs.size();i++)
	{
		if (m_objs[i]->GetType() == Index)
		{
			int p = m_objs[i]->Export(file);
			file->Write(&p,sizeof(int));
		}
	}

	// export render func
	/*if (m_render.GetNum() > 0)
	{
		hfm_chunk renderfunc = { CHUNKID_RENDERFUNC, m_render.GetSize() };
		modelchunk.size += sizeof(hfm_chunk) + renderfunc.size;
		file->Write(&renderfunc,sizeof(hfm_chunk));
		file->Write(m_render.GetData(), renderfunc.size);
	}*/

	dword ftmp = file->GetPos();
	file->SetPos(m_fpos);
	file->Write(&modelchunk,sizeof(hfm_chunk));
	file->SetPos(ftmp);

	// find streams
	printf("streams %d indexs %d\n",nStreams,nIndex);

	return m_fpos;
}

