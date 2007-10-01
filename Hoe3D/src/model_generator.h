
#ifndef _MODEL_GENERATOR_H_
#define _MODEL_GENERATOR_H_

class MaterialSystem;
class HoeModel;
//class HoeStream;
//class HoeIndex;

class ModelGenerator
{
protected:
	HoeLog * m_log;
	int m_flags;
public:
	ModelGenerator(HoeLog * log = NULL, int flags = 0);
	HoeModel * GenBox(float size);
	HoeModel * GenSphere(float size, int res);
	HoeModel * GenPlane(float size);
};

class ModelModifier : public IHoeModelModifier
{
	HoeModel * m_model;
public:
	ModelModifier(HoeModel * model);
	// funkce pro upravu materialu
	virtual void HOEAPI SetMaterialTexture(int n, const tchar * texturename);
	virtual void HOEAPI SetTextureOverlap(int n, const THoeRect & rect);

	virtual const char * GetName() { return "Model modifier";}
	virtual void Release() { delete this; }
};

#endif // _MODEL_GENERATOR_H_

