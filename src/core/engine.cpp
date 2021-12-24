#include "engine.hpp"

void Engine::tickInput(float deltaTime) {
	static float buttonDelta = 0.0f;
	int mouseDeltaX;
	int mouseDeltaY;
	buttonDelta -= deltaTime;

	if (buttonDelta < 0.0f) buttonDelta = 0.0f;

	this->inputManager->update();
	this->inputManager->mouseLocation(mouseDeltaX, mouseDeltaY);
	onMouseMove(mouseDeltaX, mouseDeltaY);

	if (this->inputManager->keyDown(SDL_SCANCODE_W)) onKeyDown(SDL_SCANCODE_W);
	if (this->inputManager->keyDown(SDL_SCANCODE_A)) onKeyDown(SDL_SCANCODE_A);
	if (this->inputManager->keyDown(SDL_SCANCODE_S)) onKeyDown(SDL_SCANCODE_S);
	if (this->inputManager->keyDown(SDL_SCANCODE_D)) onKeyDown(SDL_SCANCODE_D);
	if (this->inputManager->keyDown(SDL_SCANCODE_ESCAPE)) onKeyDown(SDL_SCANCODE_ESCAPE);

	if (this->inputManager->buttonDown(SDL_BUTTON_LEFT)) {
		if (buttonDelta == 0.0f) {
			onMouseDownL(0, 0);
			buttonDelta = 0.5f;
		}
	}
}

void Engine::cycle(float deltaTime) {
	Camera* _camera = getCamera();
	World* _world = getWorld();

	if (this->useInput) tickInput(deltaTime);

	glClear(GL_COLOR_BUFFER_BIT);

    onCycle();
	if (_world != nullptr) {
		_world->init();
	}
	if (_camera != nullptr) {
		_camera->animate(deltaTime);
	}
	if (_world != nullptr) {
		//_world->animate(deltaTime);
		_world->draw(_camera);
	}
    glColor3f(255,0,0);
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(500.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 500.0f, 0.0f);
    glVertex3f(500.0f, 500.0f, 0.0f);
    glEnd();

	swapBuffers();
}

[[noreturn]] int Engine::eventLoop() {
	SDL_Event _event;
	if (!this->timer) this->timer = new HighResTimer();
	this->timer->init();
    onInit();
	for (;;) {
		cycle(this->timer->getElapsedSeconds(1));
		while (SDL_PollEvent(&_event)) {
			this->processEvent(_event);
		}
	}
	delete timer;
	return 0;
}