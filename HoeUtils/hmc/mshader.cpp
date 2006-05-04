
#include "StdAfx.h"
#include "mshader.h"


void ModelShader::AddDefStream(const char *name)
{
	printf("Def Stream: %s\n",name);
}

void ModelShader::AddDefIndex(const char *name)
{
	printf("Def Index: %s\n",name);

}

void ModelShader::AddDefMaterial(const char *name)
{
	printf("Def Material: %s\n",name);

}

void ModelShader::AddDefInt(int num)
{
	printf("Def ip%d\n",num);
}

void ModelShader::AddDefFloat(int num)
{
	printf("Def fp%d\n",num);
}

void ModelShader::i_stdrender(InsParam *par)
{

}

int InsParam::GetNum()
{
	return 2;
}
