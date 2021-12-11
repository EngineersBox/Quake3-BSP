#include "inputManager.h"

InputManager::InputManager(const int inputs):
	this->keyboard(NULL),
	this->mouse(NULL)
{
	if (inputs & USE_KEYBOARD) this->keyboard = new Keyboard;
	if (inputs & USE_MOUSE) this->mouse = new Mouse;
}

InputManager::~InputManager() {
	if (this->keyboard) delete this->keyboard;
	if (this->mouse) delete this->mouse;
}

bool InputManager::update() {
	SDL_PumpEvents();
	if (this->keyboard) this->keyboard->update();
	if (this->mouse) this->mouse->update();
}