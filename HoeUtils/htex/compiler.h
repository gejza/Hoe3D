
#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "../lib/hoeUtils.h"
#include "../../Hoe3D/src/hoe_format.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"

class HTexBase;

class HTexCompiler
{
protected:
        HTexBase * texture; 
public:
        HTexCompiler(HTexBase * tex);
        virtual bool SaveToStream(HoeUtils::Stream * stream) = 0;
        virtual void Release() = 0;
};

#endif // _COMPILER_H_

 