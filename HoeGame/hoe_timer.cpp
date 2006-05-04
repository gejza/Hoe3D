
#include "StdAfx.h"
#include "hoe_timer.h"
#include "hoe_vars.h"

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

CTimer::CTimer(const CVar & v)
{
	var = &v;
	ztime = 0;
}

int CTimer::Compute(const float t)
{
	ztime += t;
	// odecist kolik jich prebyva
	const float d = var->GetFloat();
	int n = (int)(ztime / d);
	ztime -= d * n;
	return n;
}

END_HOEGAME

