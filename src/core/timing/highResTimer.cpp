#include "highResTimer.hpp"

bool HighResTimer::init() {
	this->startTime = SDL_GetTicks();
	return true;
}

float HighResTimer::getElapsedSeconds(unsigned long elapsedFrames) {
	Uint32 currentTime = SDL_GetTicks();
	float seconds = ((float)currentTime - (float)this->startTime) / 1000.0;
	this->startTime = currentTime;
	return seconds;
}

float HighResTimer::getFPS(unsigned long elapsedFrames) {
	Uint32 currentTime = SDL_GetTicks();
	float fps = (float)elapsedFrames * 1000.0 / ((float)currentTime - (float)this->startTime);
	this->startTime = currentTime;
	return fps;
}

float HighResTimer::lockFPS(unsigned char targetFPS) {
	if (targetFPS == 0) {
		targetFPS = 1;
	}
	Uint32 currentTime;
	float fps;
	do {
		currentTime = SDL_GetTicks();
		fps = 1000.0 / ((float)(currentTime - this->startTime));
	} while (fps > (float)targetFPS);
	this->startTime = currentTime;
	return fps;
}