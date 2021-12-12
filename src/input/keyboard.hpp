#pragma once

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>

class Keyboard {
public:
	bool keyDown(SDL_Scancode key);
	bool keyUp(SDL_Scancode key);
	bool update();
private:
	const Uint8* keys;
};

#endif // KEYBOARD_H