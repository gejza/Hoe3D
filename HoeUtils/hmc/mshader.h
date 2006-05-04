
#ifndef _MODEL_SHADER_H_
#define _MODEL_SHADER_H_

class InsParam
{
public:
	virtual int GetNum();
};

class ModelShader
{
public:
	virtual void i_stdrender(InsParam * par);
	virtual void AddDefFloat(int num);
	virtual void AddDefInt(int num);
	virtual void AddDefIndex(const char * name);
	virtual void AddDefStream(const char * name);
	virtual void AddDefMaterial(const char * name);
};

#endif // _MODEL_SHADER_H_

