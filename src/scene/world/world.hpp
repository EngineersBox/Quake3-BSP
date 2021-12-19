#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <string>

#include "../../view/camera.hpp"
#include "../../core/timing/highResTimer.hpp"
#include "../element/entity/entity.hpp"
#include "level.hpp"

class World {
protected:
	void onAnimate(float deltaTime);
	void onDraw(Camera* camera);
	void onInit();

public:
	Camera* camera;
	// AudioSystem* audioSystem;
	// std::map<std::string, Audio*> tracks;

	std::map<std::string, Entity*> entities;
    Level* level;

	World();
	World(Camera* camera);
	~World();

	void load();
	void unload();
	void animate(float deltaTime);
	void draw(Camera* camera);
	void init();
};

#endif // WORLD_H