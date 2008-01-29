
#ifndef _HOE3D_CLASS_
#define _HOE3D_CLASS_

#include "hoe.h"
#include "render_target.h"

class Hoe3DEngine : public HoeEngine 
{
	HoeRenderTarget m_rt; ///< Hlavni RenderTarget
public:
	Hoe3DEngine(int flags);
	~Hoe3DEngine();
	bool HOEAPI Init(THoeInitSettings *);
	IHoeResource * HOEAPI Create(const tchar *);
	//virtual IHoePicture * HOEAPI CreatePicture(int width, int height, dword * data);
	IHoeSystem * HOEAPI GetSystem(HOESYSTEMS sys);
	
	virtual void HOEAPI Process(const double);
	virtual bool HOEAPI Frame();
	virtual bool HOEAPI Resize(unsigned int width,unsigned int height);
	
	virtual void HOEAPI Destroy();

	virtual IHoeScene * HOEAPI CreateScene(HOE_TYPE_SCENE type);
	
};

#endif // _HOE3D_CLASS_

