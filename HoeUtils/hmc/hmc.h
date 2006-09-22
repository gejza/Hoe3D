
#ifndef _HMC_H_
#define _HMC_H_

//#include "base_obj.h"
#include "cparser.h"
#include "classes.h"

class HMC : public CParser
{
protected:
	TNamespace global; 

	int line;
	char line_buffer[512];
	HoeUtils::FileRText file;

	virtual const char * GetNextLine();
public:
	int GetActLine();
	bool Open(const char * name);

	virtual CBaseStream * CreateStream(const std::string &name, _FVF & fvf);
	virtual CBaseIndex * CreateIndex(const std::string &name);
	virtual ModelShader * CreateModelShader(const std::string &name);
	virtual CBaseMaterial * CreateMaterial(const std::string &name);
	virtual bool Finish(void);
	virtual bool Link(const char * fileout);
	virtual bool Link(HoeUtils::Stream * stream);

	virtual void Autotracking(const char * name);
};


#endif // _HMC_H_


