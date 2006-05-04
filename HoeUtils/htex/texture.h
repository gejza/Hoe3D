
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "compiler.h"
#include "texbase.h"

class HTexTexture : public HTexBase
{
	std::string path;
	std::string name;
public:
	HTexTexture();
	virtual bool Load(const char * path);
	virtual const char * GetName();
	virtual const char * GetFullPath();
	virtual const char * GetInfo();
};

#endif // _TEXTURE_H_
