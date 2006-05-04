
#include "StdAfx.h"
//#include "editor_app.h"
#include "../include/he/fs.h"
#include "../../Hoe3D/include/hfmt/hres_file.h"

bool HoeEditor::EditorFile::OpenRead(const wxString &p)
{
	path = p;
	return file.Open(path);
}

bool HoeEditor::EditorFile::OpenWrite(const wxString &p)
{
	path = p;
	return file.Open(path, wxFile::write);
}

uint HoeEditor::EditorFile::GetID()
{
	return -1;
}
/// znovuotevre soubor
bool HoeEditor::EditorFile::ReOpen()
{
	if (file.IsOpened())
		return true;
	return file.Open(path);
}

bool HoeEditor::EditorFile::IsOpen()
{
	return file.IsOpened();
}

void HoeEditor::EditorFile::Reset()
{
	file.Seek(0);
}

void HoeEditor::EditorFile::Close()
{
	file.Close();
}

const char * HoeEditor::EditorFile::GetFileName()
{
	return path.c_str();
}

void HoeEditor::EditorFile::Seek(size_t ptr)
{
	file.Seek(ptr);
}

size_t HoeEditor::EditorFile::Tell()
{
	return file.Tell();
}

void HoeEditor::EditorFile::Skip(int size)
{
	file.Seek(size, wxFromCurrent);
}

bool HoeEditor::EditorFile::Read(void * buff,size_t size)
{
	return (file.Read(buff, size) == size);
}

bool HoeEditor::EditorFile::Write(const void * buff,size_t size)
{
	return (file.Write(buff, size) == size);
}

HoeFileReader HoeEditor::EditorFile::FindChunk(uint id)
{
	HoeFileReader r(this, 0);
	hfres_header head;
	uint i;
	if (!r.Read(&head,sizeof(head)))
		goto nofind;
	if (!r.Skip(head.numres * sizeof(hfres_name)))
		goto nofind;
	for (i=0;i < head.numchunks;i++)
	{
		hfres_chunk ch;
		if (!r.Read(&ch,sizeof(ch)))
			goto nofind;;
		if (ch.chunkID == id)
		{
			return HoeFileReader(this, ch.filepos);
		}
	}
nofind:
	return HoeFileReader(NULL,0);

}

void HoeEditor::EditorFile::GetDesc(THoeFileDesc * desc)
{
	desc->fileid = GetID();
	desc->name = GetFileName();
	HoeFileReader r(this, 0);
	hfres_header head;
	r.Read(&head,sizeof(head));
	desc->resid = head.resid; ///< id resource souboru
	desc->version = head.ver; ///< verze resource souboru
}

XHoeFile * HoeEditor::EditorFS::Open(const char *fname,EHoeFileMode mode)
{
	EditorFile * f = new EditorFile();
	if (f->OpenRead(fname))
		return f;
	delete f;
	return NULL;
}

HoeFileReader HoeEditor::EditorFS::FindResource_(const char * name, dword type)
{
	// prohledat soubory
	for (uint i=0;i < this->GetFilesCount();i++)
	{
		XHoeFile * f = this->GetFileResource(i);
		if (!f)
			continue;
		HoeFileReader r(f,0);
		hfres_header head;
		r.Read(&head,sizeof(head));
		if (head.fileid != IDRESHEADER)
			continue;
		if (head.resid != type)
			continue;
		for (uint j=0;j < head.numres;j++)
		{
			hfres_name resname;
			r.Read(&resname,sizeof(resname));
			if (strcmp(resname.name, name) == 0)
				return HoeFileReader(f, resname.filepos);
		}
	}

	return HoeFileReader();

}

