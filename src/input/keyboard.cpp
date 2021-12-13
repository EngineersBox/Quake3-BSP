#include "keyboard.hpp"

bool Keyboard::keyDown(SDL_Scancode key) {
	return this->keys[key];
}

bool Keyboard::keyUp(SDL_Scancode key) {
	return !this->keys[key];
}

bool Keyboard::update() {
	this->keys = SDL_GetKeyboardState(nullptr);
	return true;
}