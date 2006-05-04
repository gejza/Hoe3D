
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "resmgr.h"
#include "hoe3d_math.h"
#include "hmap_file.h"
#include "hoe_map_part.h"
#include "camera.h"
#include "light_system.h"
#include "skybox.h"

HoeMapPart::HoeMapPart()
{
	this->skybox = NULL;
}

bool HoeMapPart::Load( HoeFile *file, XMapLoaderCallback * mlcb)
{
	FileMap::MapPart part;
	if (!file->Read(&part,sizeof(FileMap::MapPart)))
	{
		return false;
	}

	for (int i=0; i < part.num_units; i++)
	{
		FileMap::MapChunk chunk;
		if (!file->Read(&chunk,sizeof(FileMap::MapChunk)))
		{
			return false;
		}

		switch (chunk.id)
		{
		case FileMap::MapChunkSkybox:
			skybox = new SkyBox();
			if (!skybox->Load(file))
				return false;
			break;
		default:
			Con_Print("warning: unknown chunk id=%d",chunk.id);
			file->Skip(chunk.size);
			break;
		};

	}

	return true;
}

void HoeMapPart::Render(HoeCamera * cam)
{
	GetLightSystem()->Setup();

	cam->SetupMatrices();

	skybox->Render(cam);


	//
	// setup world

	// setup map
	//cam->SetupMatrices();
	//m_states->SetupMap();
	//m_map->Render(m_cam);

}

float HoeMapPart::GetHeight(const float x,const float y)				// This Returns The Height From A Height Map Index
{
	return 0;			
}


bool HoeMapPart::GetCamber(const float x1,const float x2,const float y1,const float y2, float & min,float &max)
{
	return true;
}

