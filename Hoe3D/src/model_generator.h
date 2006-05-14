
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
};


#endif // _MODEL_GENERATOR_H_

