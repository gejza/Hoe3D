
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
		vfloat total;
		vfloat percent;
		vfloat maxtime;
		vfloat maxpercent;
		inline void Start()
		{
			maxpercent = maxtime = total = percent = 0.f;
		}
		inline void Compute(const vfloat totaltime, const vfloat maxframetime)
		{
			percent = 100 * total / totaltime;
			maxpercent = 100 * maxtime / maxframetime;
			total = maxtime = 0;
		}
	};

	struct FpsStat
	{
		vfloat fps;
		vfloat maxtime;
		vfloat minfps;
		vfloat start_meter;
		int numframes;
		inline void Start(const vfloat ctime)
		{
			fps = 0.f;
			maxtime = 0.f;
			minfps = 0.f;
			numframes = 0;
			start_meter = ctime;
		}
		inline void Compute(const vfloat ctime)
		{
			const vfloat dtime = ctime - start_meter;
			fps = (vfloat)numframes / dtime;
			minfps = vfloat(1) / maxtime;
			maxtime = 0;
			numframes = 0;
			start_meter = ctime;
		}
	};

	struct Times
	{
		vfloat begin;
		vfloat preend;
		vfloat end;
	};
	
	HoeFont *font;
	bool m_show;

	FpsStat fps;
	Times times;

	vfloat stime_total;
	TimeStat s_engine;
	TimeStat s_gpu;
	TimeStat s_app;
	vfloat fpstime;

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
	virtual vfloat HOEAPI GetFPS();
	virtual bool HOEAPI LoadFont(const tchar * fontname, int height);
	virtual void HOEAPI Show(bool enable);
	void BeginFrame();
	void Publish();
	void PreEndFrame(); 
	void EndFrame();
	void ComputeStatistic();
	void SetFpsTime(vfloat ftime);
	void RegisterTimeMeter(TimeMeter * tm);

	/** Pridani trojuhelniku do pocitadla trojuhelniku */
	void AddStatTriangles(dword nt) { m_curtriangles += nt; }
	/** Pridani trojuhelniku do pocitadla trojuhelniku */
	void AddStatObjects(dword no) { m_curobjects += no; }

	static int c_showfps(int argc, const tchar * argv[], void * param);
	static int c_hidefps(int argc, const tchar * argv[], void * param);
	static int c_fpstime(int argc, const tchar * argv[], void * param);
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
