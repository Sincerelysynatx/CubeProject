#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
	Timer();
	~Timer();
	void update();
	int getFramesPerSecond();
	long getDeltaTime();
private:
	int m_framesThisSecond;
	int m_framesPerSecond;
	unsigned int m_oldTime;
	unsigned int m_newTime;
};


#endif