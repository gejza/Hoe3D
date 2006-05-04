
#ifndef _HOE_SYSTEMS_INTERFACE_H_
#define _HOE_SYSTEMS_INTERFACE_H_

#include "hoeobject.h"

enum HOESYSTEMS
{
	HOE_SYS_MATERIAL = 1,
	HOE_SYS_INFO = 2,
	HOE_SYS_INPUT = 3,
	HOE_SYS_REF = 4,
	HOE_SYS_RESMGR = 5,
	HOE_SYSTEMS_FORCE_DWORD          =0x7fffffff
};

class IHoeSystem : public IHoeInterface
{
};

// info
class IHoeInfo : public IHoeSystem
{
public:
	virtual float HOEAPI GetFPS() = 0;
	virtual bool HOEAPI LoadFont(const char * fontname, int height) = 0;
	virtual void HOEAPI Show(bool enable) = 0;
};

// material system
class IHoeMaterialSystem : public IHoeSystem
{
public:
	virtual void HOEAPI SetMaterialDir(const char * dir) = 0;
};

// file resources
class IHoeResourceMgr : public IHoeSystem
{
public:
	// 
//	virtual 
//	virtual IHoeTextureFile * HOEAPI GetTextureFile(HOE_HANDLE handle) = 0;
//	virtual IHoeModelFile * HOEAPI GetModelFile(HOE_HANDLE handle) = 0;
};

/**
 * @brief 2D engine pro vykreslovani. Neziskava se pres funkci Create, 
 *  ale engine ho preda v callbacku pro 2D kresleni.
 */
class IHoe2D : public IHoeSystem
{
public:
	virtual void SetRect(const float w,const float h) = 0;

	/// paint func
	virtual void PaintRect(const float l,const float r,const float t,const float b,unsigned long color,bool full) = 0;
	virtual	void PaintLine(float x1,float y1,float x2,float y2,unsigned long c) = 0;

	virtual void BltFast(const float l,const float r,const float t,const float b,IHoePicture *) = 0;
	virtual void Blt(const THoeRect * dest,IHoePicture *) = 0;
	virtual void Blt(const THoeRect * dest,IHoePicture *,const THoeRect * src) = 0;
	// dve moznosti alfy
	// jedna je referencni (jako co je pruhledny a co ne)
	// u te referencni je to podle hodnoty alfa v obraze
	// druha je na celej obraz, (nezalezi na obraze)
	virtual void SetAlpha(bool set) = 0;
};

class IHoeRef : public IHoeSystem
{
public:
	/** Nastavi barvu pozadi, pokud se renderuje.. ve formatu 0xFFRRGGBB */
	virtual void HOEAPI SetBackgroundColor(unsigned long color) = 0;
	virtual bool HOEAPI IsFullscreen() const = 0;
};

class IHoePaint3D : public IHoeSystem
{
public:
	virtual void ResetPos() = 0;
	virtual void Move(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void RotateY(float angle) = 0;
	virtual void Paint(IHoeModel * model) = 0;
};


#define HOEPHYS_ONLYVISIBLE		1

class IHoePhysics : public IHoeSystem
{
public:
	virtual void HOEAPI SetPart(unsigned long partID) = 0;
	virtual XHoeObject * HOEAPI RayTrace(const float *,const float *,unsigned long flags) = 0;
};

#endif // _HOE_SYSTEMS_INTERFACE_H_

