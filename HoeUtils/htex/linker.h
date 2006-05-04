
#ifndef _LINKER_H_
#define _LINKER_H_

#include <vector>
#include "../lib/hoeUtils.h"

class HTexBase;
class HTexCompiler;

class HTexLinker
{
        std::vector<HTexBase *> texs;
public:
        void Add(HTexBase * tex);
        bool Build(HoeUtils::Stream * stream);
        virtual bool Progress(int count, int cur, const char * file);
};

#endif // _LINKER_H_

