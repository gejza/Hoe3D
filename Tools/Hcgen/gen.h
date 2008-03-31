
#ifndef _HCGEN_GEN_H_
#define _HCGEN_GEN_H_

class Consts;

class CGen : public HoeUtils::IndentStream
{
public:
	CGen(HoeCore::WriteStream& s);
};

class HGen : public CGen
{
public:
	HGen(HoeCore::WriteStream& s, Consts& c);
};

#endif // _HCGEN_GEN_H_

