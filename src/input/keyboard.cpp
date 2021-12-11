#include "keyboard.h"

bool Keyboard::keyDown(SDL_Scancode key) {
	return this->keys[key];
}

bool Keyboard::keyUp(SDL_Scancode key) {
	return !this->keys[key];
}

bool Keyboard::update() {
	this->keys = SDL_GetKeyboardState(NULL);
	return true;
}