#pragma once

#ifndef G_ENGINE_H
#define G_ENGINE_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif

#include <SDL2/SDl.h>

#include "core/engine.hpp"
#include "scene/element/element.hpp"
#include "view/camera.hpp"
#include "scene/world/world.hpp"

class GEngine : public Engine {
private:
	Camera* gameCamera;
	World* gameWorld;

protected:
	Camera* onGetCamera() { return this->gameCamera; }
	World* onGetWorld() { return this->gameWorld; }

	void onInit();
	void onMouseDownL(float x, float y) override;
	void onMouseMove(int deltaX, int deltaY) override;
	void onMouseMove(int x, int y, int centerX, int centerY) override;
	void onKeyDown(SDL_Scancode nVirtKey) override;

public:
	GEngine() {
		this->gameCamera = new Camera();
		this->gameWorld = new World(this->gameCamera);
	}

	GEngine(const char* name, bool fscreen, int w, int h);

	~GEngine() override {
		delete this->gameWorld;
		delete this->gameCamera;
		this->gameWorld = nullptr;
		this->gameCamera = nullptr;
	}
};

#endif // G_ENGINE_H