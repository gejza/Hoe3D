
#include "StdAfx.h"
#include "ref.h"
#include "utils.h"
#include "filesystem.h"
#include <hoe_math.h>
#include "camera.h"
#include "texture_system.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hmap_file.h"
#include "hoe_map.h"
#include "hoe_map_part.h"


using namespace HoeMath;

/////////////////////////////////

HoeMap::HoeMap()
{
	numParts = 0;
}

void HoeMap::Clean()
{
	numParts = 0;
}

bool HoeMap::Load(const char * name, XMapLoaderCallback * mlcb)
{
	Con_Print("Load map from file '%s'",name);

	HoeFile file;
	if (!file.Open(name))
	{
		Con_Print("error open file '%s'",name);
		return false;
	}

	FileMap::HoeMapHeader head;
	if (!file.Read(&head,sizeof(FileMap::HoeMapHeader)))
	{
		Con_Print("error read header file '%s'",name);
		return false;
	}

	if (head.id != IDMAPHEADER)
	{
		Con_Print("Error read header id");
		return false;
	}
	
	if (head.ver != IDMAPVER)
	{
		Con_Print("Bad version of the map file %0.2f(%0.2f)",head.ver / 100.f,IDMAPVER/100.f);
		return false;
	}

	FileMap::MapChunk chunk;
	if (!file.Read(&chunk,sizeof(FileMap::MapChunk)))
	{
		Con_Print("Error: map empty.");
		return false;
	}

	while (1)
	{
		bool next_info = true;
		switch (chunk.id)
		{
		case FileMap::MapChunkName:
			if (chunk.size >= sizeof(this->mapname))
			{
				Con_Print("load mapname string failed.");
				return false;
			}
			file.Read(mapname,chunk.size);
			mapname[chunk.size] = '\0';
			Con_Print("Map name: %s",mapname);
			mlcb->MapInfo("name",mapname);
			break;
		case FileMap::MapChunkAutor:
			if (chunk.size >= sizeof(this->autor))
			{
				Con_Print("load autor string failed.");
				return false;
			}
			file.Read(autor,chunk.size);
			autor[chunk.size] = '\0';
			Con_Print("Autor: %s",autor);
			mlcb->MapInfo("autor",autor);
			break;
		case FileMap::MapChunkDesc:
			if (chunk.size >= sizeof(this->desc))
			{
				Con_Print("load desc string failed.");
				return false;
			}
			file.Read(desc,chunk.size);
			desc[chunk.size] = '\0';
			Con_Print("Description: %s",desc);
			mlcb->MapInfo("description",desc);
			break;
		case FileMap::MapChunkPart:
			parts[this->numParts] = new HoeMapPart;
			if (!parts[this->numParts]->Load(&file,mlcb))
				return false;
			this->numParts++;
			break;
		default:
			Con_Print("warning: unknown chunk id=%d",chunk.id);
			file.Skip(chunk.size);
		};

		if (!file.Read(&chunk,sizeof(FileMap::MapChunk)))
			break;
	}

	if (numParts == 0)
	{
		Con_Print("error: empty map");
		return false;
	}


	/*
	int x,y,xx,yy;

	m_loader = mlcb;
	Con_Print("map load '%s' - generate",name);

	// read header
	
	// read terrain header
	HoeMapTerrain map_ter;
	if (!file.Read(&map_ter,sizeof(HoeMapTerrain)))
	{
		Con_Print("error read file '%s'",name);
		return false;
	}

	m_lx = map_ter.tx;
	m_ly = map_ter.ty;
	m_nx = map_ter.num_x / map_ter.tx;
	m_ny = map_ter.num_y / map_ter.ty;

	// create leafs
	this->m_nodes = new Node[m_lx * m_ly * m_nx * m_ny];
	m_leafs = new Leaf[m_lx * m_ly];
	for (y=0;y < m_ly;y++)
		for (x=0;x < m_lx;x++)
		{
			m_leafs[y * m_lx + x].nodes = m_nodes + (y * m_lx + x) * m_nx * m_ny;
		}

	// load nodes
	for (y=0;y < m_ly;y++)
		for (yy=0;yy < m_ny;yy++)
			for (x=0;x < m_lx;x++)
				for (xx=0;xx < m_nx;xx++)
				{
					int n = (y * m_lx + x) * m_ny * m_nx + yy * m_nx + xx;
					if (!LoadNode(&m_nodes[n],&file))
					{
						Con_Print("Failed to read node");
						return false;
					}
				}

	for (y=0;y < m_ly;y++)
		for (x=0;x < m_lx;x++)
		{
			if (!CreateLeaf(&m_leafs[y * m_lx + x]))
				return false;
			m_leafs[y * m_lx + x].m_tex = this->m_texsystem->GetTexture("Grass256");
		}	*/


	file.Close();

	return true;
}

HoeMapPart * HoeMap::GetPart(const char *id)
{
	if (id == NULL)
	{
		if (this->numParts > 0)
			return this->parts[0];
		else
			return NULL;
	}
	for (int i=0; i < this->numParts;i++)
	{
		// get part name
	}

	return NULL;
}




