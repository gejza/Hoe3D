
#ifndef _HTEX_TEXTURE_BASE_H_
#define _HTEX_TEXTURE_BASE_H_

class HTexCompiler;

class HTexBase
{
public:
	virtual const char * GetName() = 0;
	virtual const char * GetFullPath() = 0;
	virtual HTexCompiler * GetCompiler();
};

#endif // _HTEX_TEXTURE_BASE_H_

