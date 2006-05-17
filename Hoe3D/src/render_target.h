
/**
   @file    render_target.h
   @date    May 2006
   @version 1.0
   @brief   Obsluha renderovani a renderingu do textury
*/

#ifndef _HOE_RENDER_TARGET_H_
#define _HOE_RENDER_TARGET_H_

#include "hoe_texture.h"

class HoeRenderTarget
{
public:
	enum Type
	{
		eMain,
		eToTexture
	};
protected:
	Type m_type;
	class HoeRenderTexture * m_tex;
#ifdef _HOE_D3D9_
	IDirect3DSurface9 * m_rt;
#endif
#ifdef _HOE_D3D8_
	IDirect3DSurface8 * m_rt;
#endif
public:
	HoeRenderTarget(Type type);
	void InitMain();
	void Setup();
	void EndRender();
	const HoeTexture * GetTexture() { return m_tex; }
};

#endif // _HOE_RENDER_TARGET_H_

