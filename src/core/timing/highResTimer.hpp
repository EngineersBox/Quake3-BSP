#pragma once

/**
 * High resolution timer.
 * Originaly authored by Dave Astle 2/1/2001
 */

#ifndef HIGH_RES_TIMER_H
#define HIGH_RES_TIMER_H

#include <SDL2/SDL.h>

class HighResTimer {
public:
	HighResTimer() {}
	~HighResTimer() {}

	bool init();
	float getElapsedSeconds(unsigned long elapsedFrames = 1);
	float getFPS(unsigned long elapsedFrames = 1);
	float lockFPS(unsigned char targetFPS);
private:
	Uint32 startTime;
};

#endif // HIGH_RES_TIMER_H