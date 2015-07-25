#include "Timer.h"
#include "Windows.h"

Timer::Timer()
{
	m_framesPerSecond = 0;
	m_framesThisSecond = 0;
}

Timer::~Timer()
{

}

void Timer::update()
{
	m_framesThisSecond++;
	m_newTime = timeGetTime();
	if (m_newTime - m_oldTime >= 1000)
	{
		m_framesPerSecond = m_framesThisSecond;
		m_framesThisSecond = 0;
		m_oldTime = timeGetTime();
	}
}

int Timer::getFramesPerSecond()
{
	return m_framesPerSecond;
}