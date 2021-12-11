#include "mouse.h"

inline bool Mouse::checkButtonInRange(int button) {
	return button >= 0 && button < NUM_MOUSE_BUTTONS;
}

bool Mouse::buttonDown(int button) {
	return checkButtonInRange(button) && this->state.buttons[button];
}

bool Mouse::buttonUp(int button) {
	return checkButtonInRange(button) && !this->state.buttons[button];
}

void Mouse::location(int& _x, int& _y) {
	_x = this->state.x;
	_y = this->state.y;
}

bool Mouse::update() {
	Uint8 buttonMask = SDL_GetRelativeMouseState(&this->state.x, &this->state.y);
	for (int i = 1; i <= NUM_MOUSE_BUTTONS; i++) {
		this->state.buttons[i] = buttonMask & SDL_BUTTON(i);
	}
	return true;
}