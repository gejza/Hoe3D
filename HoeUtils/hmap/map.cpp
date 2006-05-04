
#include "StdAfx.h"
#include "../../src/hoe3d_math.h"
#include "../../src/hmap_file.h"
#include "hmap.h"
#include "map.h"

using namespace FileMap;

Map::Map()
{
}

bool Map::Process(HoeUtils::XMLNode * cur)
{
	if (!cur->IsName("world"))
		throw "root node is not world.";

	HoeUtils::XMLNode * child = cur->GetChild();
	if (!child)
		throw "empty map.";

	do {
		if (child->IsName("mapinfo"))
		{
			this->ParseInfo(child);
			continue;
		}

		if (child->IsName("part"))
		{
			Part * p = new Part();
			p->Process(child);
			this->parts.push_back(p);
		}
	} while (child->Next());

	delete child;
	return true;
}

bool SaveString(HoeUtils::File* f, std::string &str, MapChunkType type)
{
	MapChunk chunk = { type, str.size() };
	if (!f->Write(&chunk,sizeof(MapChunk)))
		return false;
	if (!f->Write(str.c_str(),chunk.size))
		return false;
	return true;
}

bool Map::SaveToFile(HoeUtils::File* f)
{
	HoeMapHeader header = { IDMAPHEADER, IDMAPVER};
	f->Write(&header,sizeof(HoeMapHeader));

	if (this->mapname != "")
		SaveString(f,this->mapname,MapChunkName);

	if (this->autor != "")
		SaveString(f,this->autor,MapChunkAutor);

	if (this->desc != "")
		SaveString(f,this->desc,MapChunkDesc);

	for (unsigned int u=0; u < parts.size();u++)
	{
		parts[u]->SaveToFile(f);	
	}

    return true;
}

void Map::ParseInfo(HoeUtils::XMLNode * node)
{
	node = node->GetChild();
	if (!node)
		throw "emty map info.";

	do {
		if (node->IsName("title"))
		{
			mapname = node->GetText();
			printf("title: %s\n",mapname.c_str());
		}
		else if (node->IsName("autor"))
		{
			autor = node->GetText();
			printf("autor: %s\n",autor.c_str());
		}
		else if (node->IsName("desc"))
		{
			desc = node->GetText();
			printf("desc: %s\n",desc.c_str());
		}
	} while (node->Next());

	delete node;
}