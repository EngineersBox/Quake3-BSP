#pragma once

#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <glm/glm.hpp>

#include "../resource/loader/QLoader.hpp"

class CollisionHandler {
private:
	int traceType;
	float sphereRadius;
	glm::vec3 boxMin;
	glm::vec3 boxMax;
	void checkNode(int nodeIndex, float fractionInitial, float fractionFinal, glm::vec3 pinical, glm::vec3 pFinal);
	void checkBrush(QBrush* brush, glm::vec3 vOrigin, glm::vec3 vFinal);
public:
	bool collision;
	float outputFraction;

	glm::vec3 trace(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceRay(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceSphere(glm::vec3 start, glm::vec3 end, float radius);
	glm::vec3 traceBox(glm::vec3 start, glm::vec3 end, glm::vec3 min, glm::vec3 max);
};

#endif // COLLISION_HANDLER_H