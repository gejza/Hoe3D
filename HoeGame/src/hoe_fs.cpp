
#include "StdAfx.h"
#include "../include/hoe_fs.h"
#include "../include/hoe_console.h"
#include <hfmt/hres_file.h>
#include "../include/hoe_utils.h"

BEGIN_HOEGAME


uint HoeFile::idcounter = 0;

const char * HoeFile::GetOpenFunc()
{
#ifdef _WIN32
	return "CreateFile";
#else
	return "fopen";
#endif
}

HoeFile::HoeFile()
{
	name = 0;
	isopen = false;
	m_id = ++idcounter;
	//m_log = NULL;
}

/*HoeFile::HoeFile(HoeLog *log)
{
	name = 0;
	isopen = false;
	m_id = ++idcounter;
	m_log = log;
	LOG(m_log)->Log("HoeFile: create file with id=%d",m_id);
}*/

HoeFile::~HoeFile()
{
	Close();
}

bool HoeFile::Open(const char *fname,EHoeFileMode mode)
{
	isopen = true;
	m_mode = mode;

#ifdef _WIN32
	dword acces;
	dword creating;
	switch (mode)
	{
	default:
	case hftRead:
		acces = GENERIC_READ;
		creating = OPEN_EXISTING;
		break;
	case hftWrite:
		acces = GENERIC_WRITE;
		creating = OPEN_ALWAYS;
		break;
	};

	file = CreateFile(fname,acces,FILE_SHARE_READ,
		NULL,creating,0,INVALID_HANDLE_VALUE);
	if (file == INVALID_HANDLE_VALUE)
		isopen = false;
#else
	const char * m;
	switch (mode)
	{
	default:
	case hftRead:
		m = "rb";
		break;
	case hftWrite:
		m = "wb";
		break;
	};

	file = fopen(fname,m);
	if (file == NULL)
		isopen = false;
	
#endif
	/*if (isopen)
		LOG(m_log)->Log("HoeFile[%d]: %s('%s')",m_id,GetOpenFunc(),fname);
	else
		LOG(m_log)->Log("HoeFile[%d]: %s failed open %s",m_id,GetOpenFunc(),fname);
*/
	return isopen;
}

bool HoeFile::ReOpen()
{
	return Open(this->GetFileName());
}

bool HoeFile::Opended()
{
	if (!isopen && name)
		Open(this->name);

	return isopen;
}

size_t HoeFile::Read(void * buff, const size_t size)
{
	if (!Opended())
		return 0;

#ifdef _WIN32
	dword dwRead;
	ReadFile(file,buff,size,&dwRead,NULL);
	return (size_t)dwRead;
#else
	return fread(buff,1,size,file);
#endif

}

bool HoeFile::Write(const void * buff,size_t size)
{
	if (!Opended())
		return false;

#ifdef _WIN32
	dword dwWrite;
	WriteFile(file,buff,size,&dwWrite,NULL);
	if (dwWrite != size)
		return false;
#else
	if (fwrite(buff,1,size,file) != size) {
		return false;
	}
#endif
	
	return true;
}

void HoeFile::Seek(size_t ptr)
{
	if (!Opended())
		return;
#ifdef _WIN32
	SetFilePointer(file,ptr,0,FILE_BEGIN);
#else
	fseek(file,ptr,SEEK_SET);
#endif
}

size_t HoeFile::Tell()
{
#ifdef _WIN32
	return SetFilePointer(file,0,0,FILE_CURRENT);
#else
	return ftell(file);
#endif
}

bool HoeFile::Skip(size_t size)
{
	if (!Opended())
		return false;
#ifdef _WIN32
	SetFilePointer(file,size,0,FILE_CURRENT);
#else
	fseek(file,size,SEEK_CUR);
#endif
	return true;
}

void HoeFile::Reset()
{
	Seek(0);
}

void HoeFile::Close()
{
	if (!isopen)
		return;

#ifdef _WIN32
	CloseHandle(file);
	file = INVALID_HANDLE_VALUE;
#else
	fclose(file);
	file = NULL;
#endif
	isopen = false;
	//LOG(m_log)->Log("HoeFile[%d]: closed",m_id);
}

const char * HoeFile::GetFileName()
{
	if (this->name)
		return this->name;
	else
		return "NULL";
}

HoeFileReader HoeFile::FindChunk(uint id)
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

void HoeFile::GetDesc(THoeFileDesc * desc)
{
	HoeFileReader r(this, 0);
	hfres_header head;
	if (desc && r.Read(&head,sizeof(head)))
	{
		desc->fileid = this->m_id;
		desc->name = this->name;
		desc->resid = head.resid;
		desc->version = head.ver;
	}
}


//////////////////////////////////////

FileSystem::FileSystem()
{
	m_flist = NULL;
	BaseConsole::Printf("Init file system. (%s)",HoeFile::GetOpenFunc());
}

FileSystem::~FileSystem()
{
}

XHoeFile * FileSystem::Open(const char *fname,EHoeFileMode mode)
{
	HoeFile * ret = new HoeFile();
	ret->Open(fname,mode);
	return ret;
}

uint FileSystem::AddResourceFile(const char * s)
{
	HoeFile * tf = new HoeFile;
	dword type;
	if (!tf->Open(s))
	{
		BaseConsole::Printf("Error open file '%s'",s);
		//g_mem->mem_back(tf);
		delete tf;
		return 0;
	}
	tf->name = new char[strlen(s)+1];//(char*)g_mem->get_zmem(strlen(s)+1);
	strcpy(tf->name,s);

	tf->Read(&type,sizeof(dword)); // id hoe resource
	if (type != IDRESHEADER)
	{
		BaseConsole::Printf("Bad format file '%s'",s);
		//g_mem->mem_back(tf);
		delete tf;
		return 0;
	}
	tf->Read(&type,sizeof(dword));
	tf->Close();
	tf->type = type;
	tf->next = m_flist;
	m_flist = tf;
	BaseConsole::Printf("File add: %s",s);
	return tf->GetID();
}

HoeFileReader FileSystem::FindResource_(const char * name,dword type)
{
	HoeFile * nf = m_flist;
	if (type == 0) 
	{
			type = 0xffffffff;
	}
	while (nf)
	{
		if (type == nf->type || type == 0xffffffff)
		{
			// precist hlavicku
			hfres_header head;
			HoeFileReader r(nf,0);
			if (r.Read(&head, sizeof(head)))
			{
				for (uint i=0;i < head.numres;i++)
				{
					hfres_name rn;
					if (r.Read(&rn,sizeof(rn)))
					{
						if (strcmp(rn.name,name) == 0)
							return HoeFileReader(nf, rn.filepos);
					}
					else
						break;
				}
			}
		}
		nf = nf->next;
	}

	return HoeFileReader();
}

XHoeFile * FileSystem::GetFile(uint id)
{
	HoeFile * file = m_flist;
	while (file)
	{
		if (file->m_id == id)
			return file;
		file = file->next;
	}
	return NULL;
}

void FileSystem::PrintList()
{
	BaseConsole::Printf("-Files list-");
	int num = 0;

	HoeFile * f = m_flist;
	while (f)
	{
		char type[5] = {0};
		memcpy(type,&f->type,4);
		BaseConsole::Printf("Name: %s,type: %s,status: %s"
			,f->name,type,(f->IsOpen()) ? "Open":"Closed");
		num++;
		f = f->next;
	}

	if (num)
		BaseConsole::Printf("Total: %d",num);
	else
		BaseConsole::Printf("No files in resource stack.");
	BaseConsole::Printf("------------");
}

void FileSystem::CloseAll()
{
	HoeFile * f = m_flist;
	while (f)
	{
		f->Close();
		f = f->next;
	}
}

//////////////////////////////////////////////
// text file
TextFile::TextFile()
{
	file = NULL;
}

TextFile::~TextFile()
{
	Close();
}

bool TextFile::Open(const char * filename)
{
	if (IsOpen())
		Close();

	file = fopen(filename, "rt");
	return file != NULL;
}

void TextFile::Close()
{
	if (file)
	{
		fclose(file);
		file = NULL;
	}
}

bool TextFile::GetLine(char * buff, size_t size)
{
	if (!fgets(buff, size, file))
		return false;
	RemoveEndLine(buff);
	return true;
}

END_HOEGAME
