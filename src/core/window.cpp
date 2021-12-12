#include "window.h"

bool lmb = false;
bool rmb = false;
bool drag = false;

void Window::destroy() {
	delete this->inputManager;

	stopFullscreen();

	SDL_ShowCursor(SDL_ENABLE);
	SDL_DestroyWindow(this->window);

	SDL_Quit();
	exit(0);
}

void Window::resize() {
	if (this->width > this->height) {
		this->aspect = this->width;
	} else {
		this->aspect = this->height;
	}
	glViewport(0, 0, this->width, this->height);
	SDL_SetWindowSize(this->window, this->width, this->height);
}

float Window::normMousePosX(int _x) {
	return this->mouseSensitivity * (float)(_x - this->width / 2) / this->aspect;
}


float Window::normMousePosY(int _y) {
	return this->mouseSensitivity * (float)(_y - this->height / 2) / this->aspect;
}


int Window::processEvent(SDL_Event _event) {
	switch (_event.type) {
	case SDL_QUIT:
		destroy();
		return 0;

	case SDL_WINDOWEVENT_FOCUS_GAINED:
		startFullscreen(width, height);
		return 0;

	case SDL_WINDOWEVENT_FOCUS_LOST:
		stopFullscreen();
		return 0;

	case SDL_WINDOWEVENT_RESIZED:
		this->width = _event.window.data1;
		this->height = _event.window.data2;
		resize();
		return 0;

	case SDL_MOUSEBUTTONDOWN:
		switch (_event.button.button) {
		case SDL_BUTTON_LEFT:
			if (!this->useInput) {
				drag = true;
				lmb = true;
				onMouseDownL(normMousePosX(_event.button.x), normMousePosY(_event.button.y));
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (!this->useInput) {
				drag = true;
				rmb = true;
				onMouseDownR(normMousePosX(_event.button.x), normMousePosY(_event.button.y));
			}
			break;
		default:
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		if (!this->useInput) {
			int x = mouseX = _event.motion.x;
			int y = mouseY = _event.motion.y;
			int dx = _event.motion.xrel;
			int dy = _event.motion.yrel;

			onMouseMove(x, y, width, height);

			if (drag) {
				if (lmb) onMouseDragL(x, y, dx, dy);
				if (rmb) onMouseDragR(x, y, dx, dy);
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		switch (_event.button.button) {
		case SDL_BUTTON_LEFT:
			if (!useInput) {
				if (drag && !rmb) drag = false;
				lmb = false;
				onMouseUpL();
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (!useInput) {
				if (drag && !lmb) drag = false;
				rmb = false;
				onMouseUpR();
			}
			break;
		default:
			break;
		}
		break;

	case SDL_KEYUP:
		if (!useInput) onKeyUp(_event.key.keysym.scancode);
		return 0;

	case SDL_KEYDOWN:
		if (!useInput) onKeyDown(_event.key.keysym.scancode);
		return 0;

	default:
		break;
	}
	return 0;
}

void Window::startFullscreen(int width, int height) {
	if (!this->fullscreen) {
		SDL_SetWindowFullscreen(this->window, 0);
		SDL_ShowCursor(false);
		this->fullscreen = true;
	}
}


void Window::stopFullscreen() {
	if (fullscreen) {
		SDL_SetWindowFullscreen(this->window, SDL_GetWindowFlags(this->window) & SDL_WINDOW_FULLSCREEN);
		SDL_ShowCursor(true);
		this->fullscreen = false;
	}
}


Window::Window(const char* name, bool fscreen, int w, int h) {
	this->fullscreen = fscreen;
	this->width = w;
	this->height = h;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		// Sad printf :(
		fprintf(stderr, "SDL2 Video initialization failed: %s\n", SDL_GetError());
		exit(1);
	}

	this->window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w,
		h,
		SDL_WINDOW_OPENGL
	);
	if (this->window == NULL) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		exit(1);
	}

	startFullscreen(w, h);

	this->mouseSensitivity = 1.0f;
	this->inputManager = new InputManager(USE_KEYBOARD | USE_MOUSE);
	this->useInput = true;

	if (this->useInput) this->inputManager->update();

	SDL_ShowCursor(SDL_DISABLE);
}

Window::~Window() {
	destroy();
}