#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
	Timer();
	~Timer();
	void update();
	void calculateFramesPerSecond();
	void calculateDeltaTime();
	int getFramesPerSecond();
	float getDeltaTime();
private:
	int m_framesThisSecond;
	int m_framesPerSecond;
	int m_multipleTickMultiplier;
	unsigned int m_oldTimeFPS;
	unsigned int m_oldTimeDT;
	unsigned int m_newTimeFPS;
	unsigned int m_newTimeDT;
	float m_delta;
};


#endif