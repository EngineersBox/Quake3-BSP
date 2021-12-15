#include "world.hpp"

World::World() {

}

World::World(Camera* camera) {
	this->camera = camera;
}

World::~World() {
	// Delete stuff
}