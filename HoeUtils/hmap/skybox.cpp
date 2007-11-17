
#include "StdAfx.h"
#include "../../src/hoe3d_math.h"
#include "../../src/hmap_file.h"
#include "skybox.h"

using namespace FileMap;

bool SkyBox::Process(HoeUtils::XMLNode * node)
{
	// attributes
	HoeUtils::XMLNode * n = node->GetChild();

	while (n) {
		if (n->IsName("left"))
		{
			left = n->GetText();
			printf("left: %s\n",left.c_str());
		}
		else if (n->IsName("right"))
		{
			right = n->GetText();
			printf("right: %s\n",right.c_str());
		}
		else if (n->IsName("top"))
		{
			top = n->GetText();
			printf("top: %s\n",top.c_str());
		}
		else if (n->IsName("bottom"))
		{
			bottom = n->GetText();
			printf("bottom: %s\n",bottom.c_str());
		}
		else if (n->IsName("front"))
		{
			front = n->GetText();
			printf("front: %s\n",front.c_str());
		}
		else if (n->IsName("back"))
		{
			back = n->GetText();
			printf("back: %s\n",back.c_str());
		}

		if (!n->Next())
			break;
	};	

	return true;
}

void  SkyBox::Destroy()
{
	delete this;
}

#define SAVE(wall) if (skybox.wall > 0) f->Write(wall.c_str(),skybox.wall)

bool SkyBox::SaveToFile(HoeUtils::File * f)
{
	MapSkybox skybox;
	skybox.left = left.size();
	skybox.right = right.size();
	skybox.top = top.size();
	skybox.bottom = bottom.size();
	skybox.front = front.size();
	skybox.back = back.size();

	MapChunk chunk = { MapChunkSkybox, sizeof(MapSkybox) 
		+skybox.left
		+skybox.right
		+skybox.top
		+skybox.bottom
		+skybox.front
		+skybox.back };

	f->Write(&chunk, sizeof(MapChunk));
	f->Write(&skybox,sizeof(MapSkybox));

	SAVE(left);
	SAVE(right);
	SAVE(top);
	SAVE(bottom);
	SAVE(front);
	SAVE(back);

	return true;
}

