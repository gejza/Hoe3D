
#ifndef _HOE_TIMER_
#define _HOE_TIMER_

#include <hoe_core.h>

class TimeMeter : public HoeCore::TimeMeter
{
	const char * name;
public:
	TimeMeter(const char * name, bool autoregister);
	void GetInfo(char * buff);
};

bool time_initQPC();
float SysFloatTime();
float SysFrameTime();

#endif // _HOE_TIMER_
