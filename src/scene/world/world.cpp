#include "world.hpp"

World::World() {
    this->terrain = new Terrain(156, 0.5);
}

World::World(Camera* camera) {
	this->camera = camera;
    this->terrain = new Terrain(256, 0.5);
    this->level = new Level("/Users/jackkilrain/Desktop/Projects/C:C++/Quake-Id-Tech-2/models/final3.bsp");
    camera->position.y=100;
    camera->position.x=150;
    camera->position.z=0;
}

World::~World() {
	delete this->terrain;
}

void World::animate(float deltaTime) const {
    glm::vec3 newPosition = this->level->traceSphere(this->camera->prevPosition, this->camera->position, 15.0f);
    this->camera->position = newPosition;
    this->terrain->animate(deltaTime);
}

void World::draw(Camera* _camera) const {
	//for (const auto& [_, entity] : this->entities) {
	//	entity->draw(_camera);
	//}
//    this->terrain->draw(_camera);
    if (glGetError()  != GL_NO_ERROR) {
        spdlog::info("OPENGL ERROR0");
    }
    this->level->draw(_camera);
}

void World::onInit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    this->terrain->init();
}