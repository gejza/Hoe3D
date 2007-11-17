
#ifndef _DDS_COMP_H_
#define _DDS_COMP_H_

#include "compiler.h"

struct DDS_PIXELFORMAT;

class HTexCompilerDDS : public HTexCompiler
{
public:
	HTexCompilerDDS(HTexBase * tex);
	bool SaveToStream(HoeUtils::Stream * stream);
	virtual void Release() { delete this;}

	static HOEFORMAT DDSGetFormat(DDS_PIXELFORMAT * ddpf);
	static int DDSGetSize(int w,int h,DDS_PIXELFORMAT * ddpf);
	static int DDSGetPitch(int w,int h,DDS_PIXELFORMAT * ddpf);
};

#endif // _DDS_COMP_H_

