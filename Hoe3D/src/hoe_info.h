
#ifndef _HOE_INFO_
#define _HOE_INFO_



#define INFOMSG_CONST		1
#define INFOMSG_TIME		2
#define INFOMSG_SYS			4

class HoeFont;
class TimeMeter;

class InfoSystem : public IHoeInfo
{
	struct TimeStat
	{
		float total;
		float percent;
		float maxtime;
		float maxpercent;
		inline void Start()
		{
			maxpercent = maxtime = total = percent = 0.f;
		}
		inline void Compute(const float totaltime, const float maxframetime)
		{
			percent = 100.f * total / totaltime;
			maxpercent = 100.f * maxtime / maxframetime;
			total = maxtime = 0.f;
		}
	};

	struct FpsStat
	{
		float fps;
		float maxtime;
		float minfps;
		float start_meter;
		int numframes;
		inline void Start(const float ctime)
		{
			fps = 0.f;
			maxtime = 0.f;
			minfps = 0.f;
			numframes = 0;
			start_meter = ctime;
		}
		inline void Compute(const float ctime)
		{
			const float dtime = ctime - start_meter;
			fps = (float)numframes / dtime;
			minfps = 1.f / maxtime;
			maxtime = 0;
			numframes = 0;
			start_meter = ctime;
		}
	};

	struct Times
	{
		float begin;
		float preend;
		float end;
	};
	
	HoeFont *font;
	bool m_show;

	FpsStat fps;
	Times times;

	float stime_total;
	TimeStat s_engine;
	TimeStat s_gpu;
	TimeStat s_app;
	float fpstime;

	TimeMeter * tms[10];
	int ntms;

	dword m_curtriangles; ///< pocitadlo trojuhelniku
	dword m_stattriangles; ///< pocet trojuhelniku ve snimku
	dword m_curobjects; ///< pocitadlo objektu
	dword m_statobjects; ///< pocet objektu ve snimku
public:
	InfoSystem();
	~InfoSystem();
	bool Init();
	virtual float HOEAPI GetFPS();
	virtual bool HOEAPI LoadFont(const char * fontname, int height);
	virtual void HOEAPI Show(bool enable);
	void BeginFrame();
	void Publish();
	void PreEndFrame(); 
	void EndFrame();
	void ComputeStatistic();
	void SetFpsTime(float ftime);
	void RegisterTimeMeter(TimeMeter * tm);

	/** Pridani trojuhelniku do pocitadla trojuhelniku */
	void AddStatTriangles(dword nt) { m_curtriangles += nt; }
	/** Pridani trojuhelniku do pocitadla trojuhelniku */
	void AddStatObjects(dword no) { m_curobjects += no; }

	static int c_showfps(int argc, const char * argv[], void * param);
	static int c_hidefps(int argc, const char * argv[], void * param);
	static int c_fpstime(int argc, const char * argv[], void * param);
};

/*
namespace INFO_SYSTEM {
	
	bool Init();
	void ComputeFPS();
	void Publish();

	bool AddMsg(const char *,int flags);
}; // namespace INFO_SYSTEM

#define info INFO_SYSTEM
  */

#endif // _HOE_INFO_
