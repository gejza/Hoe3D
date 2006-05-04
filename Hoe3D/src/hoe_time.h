
#ifndef _HOE_TIMER_
#define _HOE_TIMER_

class TimeMeter
{
	const char * name;
	float start;
	float max;
	float total;
	float avarage;
	float percent;
	float pubmax;
	int num;
public:
	TimeMeter(const char * name, bool autoregister);
	void Compute(const float totaltime)
	{
		percent = 100.f * total / totaltime;
		avarage = (total / num) * 1000;
		pubmax = max * 1000;
		num = 0;
		max = 0;
		total = 0.f;
	}
	void GetInfo(char * buff);
	void Begin();
	void End();
};

bool time_initQPC();
float SysFloatTime();
float SysFrameTime();

#endif // _HOE_TIMER_
