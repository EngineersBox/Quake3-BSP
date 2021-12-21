#include "world.hpp"

World::World() = default;

World::World(Camera* camera) {
	this->camera = camera;
    this->level = new Level("/Users/jackkilrain/Desktop/Projects/C:C++/Quake-Id-Tech-2/models/final3.bsp");
    camera->position.y=0;
    camera->position.x=0;
    camera->position.z=0;
}

World::~World() {
	// Delete stuff
}

void World::animate(float deltaTime) const {
    glm::vec3 newPosition = this->level->traceSphere(this->camera->prevPosition, this->camera->position, 15.0f);
    this->camera->position = newPosition;
}

void World::draw(Camera* _camera) const {
	//for (const auto& [_, entity] : this->entities) {
	//	entity->draw(_camera);
	//}
    this->level->draw(_camera);
}

void World::init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
}