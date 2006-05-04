
#ifndef _HOEGAME_TIMER_H_
#define _HOEGAME_TIMER_H_

#include <hoe3d.h>
#include "hoe_game.h"

BEGIN_HOEGAME

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

/*!!!*/
// trida ktera spousti funkci 1x za urcity cas
class CVar;

class CTimer
{
	float ztime; // cas k pripocteni
	union {
		float dtime;
		const CVar * var;
	};
	int flags;
public:
	CTimer(const CVar & v);
	// funkce dostane cas a vrati pocet spusteni funkce
	int Compute(const float t);
};

END_HOEGAME

#endif // _HOEGAME_TIMER_H_

