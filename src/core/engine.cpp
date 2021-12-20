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
	Camera* camera = getCamera();
	World* world = getWorld();

	if (this->useInput) tickInput(deltaTime);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	onInit();
	if (world != nullptr) {
		world->init();
	}
	if (camera != nullptr) {
		camera->animate(deltaTime);
	}
	if (world != nullptr) {
		//world->animate(deltaTime);
		world->draw(camera);
	}

	swapBuffers();
}

[[noreturn]] int Engine::eventLoop() {
	SDL_Event _event;
	if (!this->timer) this->timer = new HighResTimer();
	this->timer->init();
	for (;;) {
		cycle(this->timer->getElapsedSeconds(1));
		while (SDL_PollEvent(&_event)) {
			this->processEvent(_event);
		}
	}
	delete timer;
	return 0;
}