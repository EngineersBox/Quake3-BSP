#include "world.hpp"

World::World() {

}

World::World(Camera* camera) {
	this->camera = camera;
}

World::~World() {
	// Delete stuff
}

void World::draw(Camera* camera) {
	//for (const auto& [_, entity] : this->entities) {
	//	entity->draw(camera);
	//}
}

void World::init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
}