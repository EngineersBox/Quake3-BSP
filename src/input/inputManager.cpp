#include "inputManager.hpp"

InputManager::InputManager(int inputs):
	keyboard(nullptr),
	mouse(nullptr)
{
	if (inputs & USE_KEYBOARD) this->keyboard = new Keyboard();
	if (inputs & USE_MOUSE) this->mouse = new Mouse();
}

InputManager::~InputManager() {
	delete this->keyboard;
	delete this->mouse;
}

bool InputManager::update() {
	SDL_PumpEvents();
	if (this->keyboard) this->keyboard->update();
	if (this->mouse) this->mouse->update();
	return true;
}