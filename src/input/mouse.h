#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include <SDL2/SDL.h>

/*
SDL_BUTTON_LEFT       = 1
SDL_BUTTON_MIDDLE     = 2
SDL_BUTTON_RIGHT      = 3
SDL_BUTTON_WHEELUP    = 4
SDL_BUTTON_WHEELDOWN  = 5
*/
#define NUM_MOUSE_BUTTONS 5

struct MouseState {
	int x;
	int y;
	bool buttons[NUM_MOUSE_BUTTONS + 1];
};

class Mouse {
public:
	bool buttonDown(int button);
	bool buttonUp(int button);
	void location(int& _x, int& _y);
	bool update();
private:
	inline bool checkButtonInRange(int button);
	MouseState state;
};

#endif // MOUSE_H