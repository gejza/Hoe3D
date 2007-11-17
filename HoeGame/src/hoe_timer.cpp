
#include "StdAfx.h"
#include "../include/hoe_timer.h"
#include "../include/hoe_vars.h"

BEGIN_HOEGAME

Timer::Timer()
{
	m_pause = true;
}

void Timer::Set(TTimerMode mode, int sec)
{
	m_mode = mode;
	m_start = (float)sec;
	if (m_mode == tmClock)
		m_alarm = -1;
	else
        m_alarm = 0;
}

void Timer::Update(float time)
{
	if (IsPaused())
		return;

	bool alarm = false;
	float nt;

	switch (m_mode)
	{
	case tmClock:
		nt = m_time + time;
		if (m_time >= m_alarm && m_alarm <= nt)
			alarm = true;
		break;
	case tmMeter:
		nt = m_time - time;
		if (m_time >= m_alarm && m_alarm <= nt)
			alarm = true;
		break;
	default:
		assert(false && "unknown timer mode");
	};

	m_time = nt;
	if (alarm)
		this->OnAlarm(m_alarm);
}

void Timer::Start()
{
	if (IsPaused())
        m_pause = false;
	else
		m_time = m_start;
}

void Timer::Stop()
{
	m_pause = true;
}

void Timer::Pause()
{
	m_pause = true;
}

CTimer::CTimer()
{
	m_t = m_btime = 0.f;
	m_isvtime = false;
	m_isrun = false;
}

void CTimer::Start(const float btime, bool randomrandomstart)
{
	m_btime = btime;
	m_isvtime = false;
	// nastaveni start hodnoty
	m_t = randomrandomstart ? m_btime * HoeCore::RandFloat(0.6f,1.f): m_btime;
	m_isrun = true;
}

void CTimer::Start(const CVar & v, bool randomstart)
{
	m_vtime = &v;
	m_isvtime = true;
	// nastaveni start hodnoty
	if (randomstart)
		m_t = m_vtime->GetFloat() * HoeCore::RandFloat(0.6f,1.f);
	else
		m_t = m_vtime->GetFloat();
	m_isrun = true;
}

void CTimer::Stop()
{
	m_isrun = false;
}

int CTimer::Update(const float t)
{
	if (!m_isrun)
		return 0;
	m_t -= t;
	if (m_t > 0.f) // jeste neni cas
		return 0;
	register float start;
	if (m_isvtime)
		start = m_vtime->GetFloat();
	else
		start = m_btime;

	int passes = (int)(-m_t/start);
	passes++;
	m_t += start * passes;

	return passes;
}

END_HOEGAME

