#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <string>

#include "../../view/camera.hpp"
#include "../../core/timing/highResTimer.hpp"
#include "../element/entity/entity.hpp"
#include "level.hpp"
#include "../element/terrain/terrain.hpp"

class World {
protected:
	void onAnimate(float deltaTime);
	void onDraw(Camera* camera);
	void onInit();

public:
	Camera* camera;
    Terrain* terrain;
	// AudioSystem* audioSystem;
	// std::map<std::string, Audio*> tracks;

	std::map<std::string, Entity*> entities;
    Level* level;

	World();
	explicit World(Camera* camera);
	~World();

	void load();
	void unload();
	void animate(float deltaTime) const;
	void draw(Camera* _camera) const;
    void init() { onInit(); };
};

#endif // WORLD_H