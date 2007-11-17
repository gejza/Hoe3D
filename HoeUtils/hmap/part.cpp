
#include "StdAfx.h"
#include "../../src/hoe3d_math.h"
#include "../../src/hmap_file.h"
#include "part.h"
#include "skybox.h"

using namespace FileMap;

Part::Part()
{

}

bool Part::Process(HoeUtils::XMLNode * node)
{
	id = node->GetAttr("id");
	printf("map (%s)\n",id.c_str());
	
	HoeUtils::XMLNode * n = node->GetChild();
	do {
		if (n->IsName("skybox"))
		{
			SkyBox * sb = new SkyBox;
			sb->Process(n);
			this->units.push_back(sb);
		}
	} while (n->Next());
		delete n;
	return true;
}

bool Part::SaveToFile(HoeUtils::File * f)
{
	MapChunk chunk;
	chunk.id = FileMap::MapChunkPart;

	chunk.size = sizeof(MapPart);
	f->Write(&chunk, sizeof(MapChunk));

	MapPart mp;
	strncpy(mp.id,id.c_str(),sizeof(mp.id));
	mp.num_units = this->units.size();
	f->Write(&mp,chunk.size);

	for (unsigned int u=0;u < this->units.size();u++)
	{
		units[u]->SaveToFile(f);
	}
	return true;
}

