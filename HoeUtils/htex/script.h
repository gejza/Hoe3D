
#ifndef _SCRIPT_H_
#define _SCRIPT_H_

class ScriptParser
{

public:
        virtual bool Parse();
        virtual const char * GetNextLine() = 0;
};

#endif // _SCRIPT_H_




