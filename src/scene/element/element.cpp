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

void Element::handleCollisions(Element* element) {
	if (((element->position - this->position).length() <= (element->size + this->size)) && (element != ((Element*)this))) {
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