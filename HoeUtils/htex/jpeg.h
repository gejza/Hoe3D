
#ifndef _JPEG_H_
#define _JPEG_H_

#include "compiler.h"

class HTexCompilerJPG : public HTexCompiler
{
public:
	HTexCompilerJPG(HTexBase * tex);
	bool SaveToStream(HoeUtils::Stream * stream);
	virtual void Release() { delete this;}

};

#endif // _JPEG_H_


