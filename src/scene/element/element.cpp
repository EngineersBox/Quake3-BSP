#include "element.hpp"

void Element::draw(Camera* camera) {
	glPushMatrix();
	onDraw(camera);
	if (hasChild()) {
		((Element*)this->child)->draw(camera);
	}
	glPopMatrix();
	if (hasParent() && !isLastChild()) {
		((Element*)this->next)->draw(camera);
	}
}

void Element::animate(float deltaTime) {
	onAnimate(deltaTime);
	if (hasChild()) {
		((Element*)this->child)->animate(deltaTime);
	}
	if (hasParent() && !isLastChild()) {
		((Element*)this->next)->animate(deltaTime);
	}
	if (isDead) {
		delete this;
	}
}

inline float vecLength(glm::vec3 vec) {
    return (float)sqrt((double)(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

void Element::handleCollisions(Element* element) {
	if ((vecLength(element->position - this->position) <= (element->size + this->size)) && (element != ((Element*)this))) {
		onCollision(element);
		if (hasChild()) {
			((Element*)this->child)->handleCollisions(element);
		}
		if (hasParent() && !isLastChild()) {
			((Element*)this->next)->handleCollisions(element);
		}
	}
	if (element->hasChild()) {
		handleCollisions((Element*)(element->child));
	}
	if (element->hasParent() && !element->isLastChild()) {
		handleCollisions((Element*)(element->next));
	}
}

void Element::init() {
	onInit();
	if (hasChild()) {
		((Element*)this->child)->init();
	}
	if (hasParent() && !isLastChild()) {
		((Element*)this->next)->init();
	}
}

Element* Element::findRoot() {
	return this->parent ? ((Element*)this->parent)->findRoot() : this;
}