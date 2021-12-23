#include "g_engine.hpp"

GEngine::GEngine(const char* name, bool fscreen, int w, int h) :
	Engine(name, fscreen, w, h)
{
	this->gameCamera = new Camera();
	this->gameWorld = new World(this->gameCamera);
	this->gameCamera->yaw = 45.0;
}


void GEngine::onInit() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_COLOR_MATERIAL);
	glViewport(0, 0, this->width, this->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(54.0f, (float)this->width / (float)this->height, 0.1f, 5000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void GEngine::onMouseDownL(float x, float y) {
	// TODO
}

constexpr float velocityIncDelta = 1.5f;

void GEngine::onKeyDown(SDL_Scancode nVirtKey) {
	switch (nVirtKey) {
	case SDL_SCANCODE_W:
		this->gameCamera->velocity += glm::vec3(0.0, 0.0, velocityIncDelta);
		break;
	case SDL_SCANCODE_S:
		this->gameCamera->velocity += glm::vec3(0, 0, -velocityIncDelta);
		break;
	case SDL_SCANCODE_A:
		this->gameCamera->velocity += glm::vec3(-velocityIncDelta, 0.0, 0.0);
		break;
	case SDL_SCANCODE_D:
		this->gameCamera->velocity += glm::vec3(velocityIncDelta, 0.0, 0.0);
		break;
    case SDL_SCANCODE_SPACE:
        this->gameCamera->moveTo(
            this->gameCamera->position + glm::vec3(0.0, velocityIncDelta, 0.0)
        );
	case SDL_SCANCODE_F:
		if (this->fullscreen) this->stopFullscreen();
		else this->startFullscreen(this->width, this->height);
		break;
	case SDL_SCANCODE_G:
		while (!this->inputManager->keyDown(SDL_SCANCODE_H)) this->inputManager->update();
		break;
	case SDL_SCANCODE_ESCAPE:
		SDL_Event event;
		event.type = SDL_QUIT;
		event.quit.type = SDL_QUIT;
		SDL_PushEvent(&event);
		break;
	case SDL_SCANCODE_EQUALS:
		this->mouseSensitivity += 0.05f;
		break;
	case SDL_SCANCODE_MINUS:
		this->mouseSensitivity -= 0.05f;
		if (this->mouseSensitivity < 0.05)
			this->mouseSensitivity = 0.05f;
		break;
	default:
		break;
	}
}


void GEngine::onMouseMove(int deltaX, int deltaY) {
	this->gameCamera->yaw += deltaX * this->mouseSensitivity;
	this->gameCamera->pitch -= deltaY * this->mouseSensitivity;
}


void GEngine::onMouseMove(int x, int y, int centerX, int centerY) {
	static float oldX;
	static float oldY;
	static float yaw = 0.0f;
	static float pitch = 0.0f;

	auto mX = (float)x;
	auto mY = (float)y;

	if (mX < centerX / 2.0)
		this->gameCamera->yaw -= 0.25f * this->mouseSensitivity;
	else if (mX > centerX / 2.0)
		this->gameCamera->yaw += 0.25f * this->mouseSensitivity;

	if (mY < centerY / 2.0)
		this->gameCamera->pitch += 0.25f * this->mouseSensitivity;
	else if (mY > centerY / 2.0)
		this->gameCamera->pitch -= 0.25f * this->mouseSensitivity;

	oldX = mX;
	oldY = mY;
}