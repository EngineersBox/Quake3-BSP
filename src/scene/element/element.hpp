#pragma once

/**
 * Element for tree based scene elements
 * Originaly authored by Kevin Hawkins 29/3/2001
 */

#ifndef ELEMENT_H
#define ELEMENT_H

#include <glm/glm.hpp>

#include "../structure/node.hpp"
#include "../../view/camera.hpp"

class Element : public Node {
protected:
	virtual void onAnimate(float deltaTime) {
		this->position += this->velocity * deltaTime;
		this->velocity += this->acceleration * deltaTime;
	}
	virtual void onDraw(Camera* camera) {}
	virtual void onCollision(Element* element) {}
	virtual void onInit() {
		handleCollisions(findRoot());
	}
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float size;
	bool isDead;

	Element() { isDead = false; }
	~Element() override = default;

	virtual void load() {}
	virtual void unload() {}

	void draw(Camera* camera);
	void animate(float deltaTime);
	void handleCollisions(Element* element);
	void init();
	Element* findRoot();
};

#endif // ELEMENT_H