
#ifndef _HOE_VIDEO_PLAYER_H_
#define _HOE_VIDEO_PLAYER_H_

#include "hoe_texture.h"

class HoeVideoPlayer 
{
public:
	class HoeVideoTexture * m_tex;
public:
	HoeVideoPlayer();
	bool Load(const char * fname);
	void NextFrame();
	const HoeTexture * GetTexture() { return m_tex; }
};

#endif // _HOE_VIDEO_PLAYER_H_

