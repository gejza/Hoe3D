
#include "StdAfx.h"
#include "../include/hoe_benchmark.h"

BEGIN_HOEGAME

void Bench::Start(const tchar * name)
{
#ifdef _WIN32
	MessageBox(GetActiveWindow(), name, T("Benchmark"), 0);
	start = timeGetTime();
#endif
}

void Bench::End(int num)
{
#ifdef _WIN32
	DWORD t = timeGetTime() - start;
	HoeCore::String_s<1024> buff;
	buff.printf(T("Stat: total: %f avg: %f 30fps: %f %%"),t * 0.001f, (float)t / num * 0.001f,
		t * 3.f / num);
	MessageBox(GetActiveWindow(), buff, T("Benchmark"), 0);

	// 1/30 -- cas jednoho snimku
	// num / 30 -- cas num snimku
	// (t * 3
	// (num)
#endif
}



END_HOEGAME

