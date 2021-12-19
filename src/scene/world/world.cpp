#include "world.hpp"

World::World() {

}

World::World(Camera* camera) {
	this->camera = camera;
    this->level = new Level("/Users/jackkilrain/Desktop/Projects/C:C++/Quake-Id-Tech-2/models/final3.bsp");
}

World::~World() {
	// Delete stuff
}

void World::animate(float deltaTime) {
    glm::vec3 newPosition = this->level->traceSphere(this->camera->prevPosition, this->camera->position, 15.0f);
    this->camera->position = newPosition;
}

void World::draw(Camera* camera) {
	//for (const auto& [_, entity] : this->entities) {
	//	entity->draw(camera);
	//}
    this->level->draw(camera);
}

void World::init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
}