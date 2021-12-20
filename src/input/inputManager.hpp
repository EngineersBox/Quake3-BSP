#pragma once

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>

#include "keyboard.hpp"
#include "mouse.hpp"

#define USE_KEYBOARD 0x1
#define USE_MOUSE 0x2

class InputManager {
public:
	explicit InputManager(int inputs);
	~InputManager();

	Keyboard* getKeyboard() { return this->keyboard; }
	Mouse* getMouse() { return this->mouse; }

	bool update();

	bool keyDown(SDL_Scancode key) { return this->keyboard && this->keyboard->keyDown(key); }
	bool keyUp(SDL_Scancode key) { return this->keyboard && this->keyboard->keyUp(key); }
	
	bool buttonDown(int button) { return this->mouse && this->mouse->buttonDown(button); }
	bool buttonUp(int button) { return this->mouse && this->mouse->buttonUp(button); }
	
	void mouseLocation(int& _x, int& _y) { this->mouse->location(_x, _y); }
private:
	Keyboard* keyboard;
	Mouse* mouse;
};

#endif // INPUT_MANAGER_H