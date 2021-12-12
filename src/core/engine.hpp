#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "window.hpp"
#include "../view/camera.hpp"
#include "timing/highResTimer.hpp"

class Engine : public Window {
protected:
	HighResTimer* timer;

	virtual void gameLoop(float deltaTime);
	virtual void onPrepare() {}
	virtual Camera* getCamera() { return NULL;  }
	//virtual World* getWorld() { return NULL;  }
	virtual void tickInput(float deltaTime);
	void swapBuffers() { SDL_GL_SwapWindow(this->window); }
public:
	Engine() {}
	Engine(const char* name, bool fscreen, int width, int height) :
		Window(name, fscreen, width, height) {}
	~Engine() {}
	int startLoop();
};

#endif // ENGINE_H