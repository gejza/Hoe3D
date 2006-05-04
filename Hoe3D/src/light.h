
#ifndef _LIGHT_H_
#define _LIGHT_H_

class DebugObject
{
public:
	void Render();
};

class HoeLight : public IHoeLight
{
#ifdef _HOE_D3D_
	D3DLight light;
#endif
#ifdef _HOE_OPENGL_
	HoeMath::VECTOR4 pos;
	HoeMath::VECTOR4 color;
#endif
	DebugObject obj;
public:
	HoeLight();
	void Set(int slot);
	virtual void HOEAPI SetPosition(const float x, const float y, const float z);
	virtual void HOEAPI SetColor(const float r, const float g, const float b);
	// 
	HoeLight * next;
};

#endif // _LIGHT_H_

 