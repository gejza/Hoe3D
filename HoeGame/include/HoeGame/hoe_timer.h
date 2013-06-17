
#ifndef _HOEGAME_TIMER_H_
#define _HOEGAME_TIMER_H_

#include <hoe3d.h>
#include "hoe_game.h"

namespace HoeGame {

class StopWatch
{
	float m_time;
public:
	StopWatch() { m_time = 0.f; }
	inline void Reset() { m_time = 0.f; }
	bool AddTime(const float t, const float fin) { m_time+=t; return m_time >= fin; }
};

class Timer
{
public:
	enum TTimerMode
	{
		tmClock,
		tmMeter,
	};
protected:
	TTimerMode m_mode;
	bool m_pause;
	float m_time;
	float m_start;
	float m_alarm;
public:
	Timer();
	void Set(TTimerMode mode, int sec);
	void SetAlarm(float time);
	void Update(float time);
	void Start();
	void Stop();
	void Pause();
	bool IsPaused() { return m_pause; }
	virtual void OnAlarm(float time) {};
	int GetMinutes() { return ((int)m_time) / 60; }
	int GetSeconds() { return ((int)m_time) % 60; }
};

// casovy cyklovac
class CVar;

class CTimer
{
	float m_t;
	union {
		float m_btime;
		const CVar * m_vtime;
	};
	bool m_isvtime;
	bool m_isrun;
public:
	CTimer();
	void Start(const float btime, bool randomrandomstart = false);
	void Start(const CVar & v, bool randomstart = false);
	void Stop();
	// funkce dostane cas a vrati pocet spusteni funkce
	int Update(const float t);
};



} // namespace HoeGame

#endif // _HOEGAME_TIMER_H_

