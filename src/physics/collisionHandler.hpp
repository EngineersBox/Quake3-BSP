#pragma once

#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <glm/glm.hpp>

class CollisionHandler {
public:
	bool collision;
	float outputFraction;

	glm::vec3 trace(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceRay(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceSphere(glm::vec3 start, glm::vec3 end, float radius);
	glm::vec3 traceBox(glm::vec3 start, glm::vec3 end, glm::vec3 min, glm::vec3 max);
};

#endif // COLLISION_HANDLER_H