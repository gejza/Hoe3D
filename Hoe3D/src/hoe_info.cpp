
#include "StdAfx.h"
#include "ref.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "hoe_time.h"
#include "hoe_font.h"
#include "config.h"
#include "camera.h"
#include "hoe_info.h"
#include "shared.h"

InfoSystem::InfoSystem()
{
	SET_SHARED_PTR(info);

	fps.Start(SysFloatTime());
	s_engine.Start();
	s_gpu.Start();
	s_app.Start();
	stime_total = 0.f;
	
	font = NULL;

	ntms = 0; memset(tms,0,sizeof(tms));

	fpstime = 1.f;

	GetExec()->Register(T("showfps"),InfoSystem::c_showfps,NULL);
	GetExec()->Register(T("hidefps"),InfoSystem::c_hidefps,NULL);
	GetExec()->Register(T("setfpstime"),InfoSystem::c_fpstime,NULL);
	m_show = false;

	m_stattriangles = 0;
}

InfoSystem::~InfoSystem()
{
	UNSET_SHARED_PTR(info);
}

bool InfoSystem::Init()
{

	return true;
}

bool InfoSystem::LoadFont(const tchar * fontname, int height)
{
	font = new HoeFont(fontname,height, 3.f);
	if (!font->Init())
	{
		delete font;
		font = NULL;
		m_show = false;
		return false;
	}
	Show(true);
	return true;
}

void InfoSystem::Show(bool show)
{
	if (font && show)
		m_show = true;
	else
		m_show = false;
}

float InfoSystem::GetFPS()
{
	return fps.fps;
}

void InfoSystem::ComputeStatistic()
{
	for (int i=0;i < ntms;i++)
	{
		if (tms[i] != NULL)
			tms[i]->ComputeFrame(stime_total);
	}

	s_engine.Compute(stime_total,fps.maxtime);
	s_gpu.Compute(stime_total,fps.maxtime);
	s_app.Compute(stime_total,fps.maxtime);

	stime_total = 0.f;

	fps.Compute(SysFloatTime());
}

void InfoSystem::BeginFrame()
{
	float time_s = SysFloatTime();

	// spocitani
	const float td = time_s - times.begin;

	stime_total += td;
	s_engine.total += times.preend - times.begin;
	s_gpu.total += times.end - times.preend;
	s_app.total += time_s - times.end;


	if (td > fps.maxtime) // nejvyssi cas snimku
	{
		fps.maxtime = td;
		s_engine.maxtime = times.preend - times.begin;
		s_gpu.maxtime = times.end - times.preend;
		s_app.maxtime = time_s - times.end;
	}

	times.begin = time_s;
	fps.numframes++;

	m_curtriangles = 0;
	m_curobjects = 0;

	if (stime_total > fpstime)
	{
		ComputeStatistic();
	}
}

void InfoSystem::PreEndFrame()
{
	times.preend = SysFloatTime();
}

void InfoSystem::EndFrame()
{
	times.end = SysFloatTime();
	m_stattriangles = m_curtriangles;
	m_statobjects = m_curobjects;
}

void InfoSystem::Publish()
{
#ifndef UNICODE
	if (m_show)
	{
		float h = font->GetTextHeight();

		assert(font);
		HoeCamera::Setup2DMatrices(0,0);
		char buff[100];
		float y = 15;
		sprintf(buff,"FPS: %.3f (e: %.2f%% g:%.2f%% a:%.2f%%), tri: %d, o: %d",
			fps.fps,s_engine.percent,s_gpu.percent,s_app.percent,m_stattriangles,m_statobjects);
		font->DrawText(10,y,0xffffffff,buff,HOEFONT_BACK);
		y += 2 + h;
		sprintf(buff,"FPS: %.3f (e: %.2f%% g:%.2f%% a:%.2f%%)",fps.minfps,s_engine.maxpercent, s_gpu.maxpercent, s_app.maxpercent);
		font->DrawText(10,y,0xffffffff,buff,HOEFONT_BACK);
		y += 2 + h;
		font->DrawText(10,y,0xffffffff,Config::GetHoeDesc(),HOEFONT_BACK);
		y += 2 + h;
		for (int i=0;i < ntms;i++)
		{
			if (tms[i])
			{
				tms[i]->GetInfo(buff);
				font->DrawText(10,y,0xffffffff,buff,HOEFONT_BACK);
				y += 2 + h;
			}
		}
		// dalsi debug info
	}
#endif
}

void InfoSystem::SetFpsTime(float ftime)
{
	if (ftime > 0.f && ftime < 3600.f)
		fpstime = ftime;
}

void InfoSystem::RegisterTimeMeter(TimeMeter * tm)
{
	tms[ntms++] = tm;
}

/////////////////////////////////////////////////////////

int InfoSystem::c_showfps(int argc, const tchar * argv[], void * param)
{
	GetInfo()->Show(true);
	return 0;
}

int InfoSystem::c_hidefps(int argc, const tchar * argv[], void * param)
{
	GetInfo()->Show(false);
	return 0;
}

int InfoSystem::c_fpstime(int argc, const tchar * argv[], void * param)
{
	if (argc != 2)
		return 1;

	GetInfo()->SetFpsTime((float)HoeCore::string::atof(argv[1]));

	return 0;
}



