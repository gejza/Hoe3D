
#ifndef _PNG_COMPILER_H_
#define _PNG_COMPILER_H_

#include "compiler.h"

class HTexCompilerPNG : public HTexCompiler
{
public:
	HTexCompilerPNG(HTexBase * tex);
	bool SaveToStream(HoeUtils::Stream * stream);
	virtual void Release() { delete this;}

};

#endif // _PNG_COMPILER_H_


