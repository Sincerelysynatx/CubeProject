#include "Timer.h"
#include "Windows.h"
#include <iostream>

Timer::Timer()
{
	m_framesPerSecond = 0;
	m_framesThisSecond = 0;
	m_delta = 0;
	m_oldTimeFPS = 0;
	m_newTimeFPS = 0;
	m_oldTimeDT = 0;
	m_newTimeDT = 0;
}

Timer::~Timer()
{

}

void Timer::update()
{
	calculateFramesPerSecond();
	calculateDeltaTime();
}

void Timer::calculateFramesPerSecond()
{
	m_newTimeFPS = timeGetTime();
	m_framesThisSecond++;
	if (m_newTimeFPS - m_oldTimeFPS >= 1000)
	{
		m_framesPerSecond = m_framesThisSecond;
		m_framesThisSecond = 0;
		m_oldTimeFPS = timeGetTime();
	}
}

void Timer::calculateDeltaTime()
{
	m_newTimeDT = timeGetTime();
	if (m_newTimeDT > m_oldTimeDT)
	{
		m_delta = (float) (m_newTimeDT - m_oldTimeDT) / 16;
		m_oldTimeDT = m_newTimeDT;
	}
}

int Timer::getFramesPerSecond()
{
	return m_framesPerSecond;
}

float Timer::getDeltaTime()
{
	return m_delta;
}