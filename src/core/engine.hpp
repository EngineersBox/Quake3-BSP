#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "window.hpp"
#include "../view/camera.hpp"
#include "timing/highResTimer.hpp"
#include "../scene/world/world.hpp"

class Engine : public Window {
private:
    Camera* camera;
    World* world;
protected:
	HighResTimer* timer;

	virtual void cycle(float deltaTime);
	virtual void onInit() {}
	virtual Camera* getCamera() { return this->camera;  }
	virtual World* getWorld() { return this->world;  }
	virtual void tickInput(float deltaTime);
	void swapBuffers() { SDL_GL_SwapWindow(this->window); }
public:
	Engine(): timer(new HighResTimer) {
        this->camera = new Camera();
        this->world = new World(this->camera);
	}
	Engine(const char* name, bool fscreen, int width, int height) :
		Window(name, fscreen, width, height),
		timer(new HighResTimer) {
        this->camera = new Camera();
        this->world = new World(this->camera);
    }
	~Engine() override = default;

    [[noreturn]] int eventLoop();
};

#endif // ENGINE_H