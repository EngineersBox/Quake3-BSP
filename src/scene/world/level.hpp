#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif

#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <spdlog/spdlog.h>

#include "../../resource/loader/QLoader.hpp"
#include "../../resource/material/texture.hpp"
#include "../../view/camera.hpp"
#include "../../view/frustum.hpp"
#include "../element/element.hpp"

class Level {
private:
	QMapQ3 map;
	std::vector<Texture> lightmaps;
	std::vector<Texture> albedos;

	void convertEdges();
	void drawTriangle(int faceIndex);
	void drawTriangleVA(int faceIndex);
	void drawTriangleMesh(int faceIndex);

	// BSP Related
	int findLeaf(const glm::vec3& camperaPos);
	bool clusterVisible(int actual, int leaf);
	void generateLightmaps();
	void generateAlbedos();

	// Collisions
	int traceType;
	float sphereRadius;
	glm::vec3 boxMin;
	glm::vec3 boxMax;
	void checkNode(int nodeIndex, float fractionInitial, float fractionFinal, glm::vec3 pinical, glm::vec3 pFinal);
	void checkBrush(QBrush* brush, glm::vec3 vOrigin, glm::vec3 vFinal);
protected:
	void onAnimate(float deltaTime) {}
	void onDraw(Camera* camera);
	void onCollision(Element* element);
public:
	// glActiveTextureARB -> glActiveTexture
	// glClientActiveTextureARB -> glClientActiveTexture

	int triangles;
	Frustum frustum;

	// Collisions
	bool collision;
	float outputFraction;

	glm::vec3 trace(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceRay(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceSphere(glm::vec3 start, glm::vec3 end, float radius);
	glm::vec3 traceBox(glm::vec3 start, glm::vec3 end, glm::vec3 min, glm::vec3 max);

	Level(const char* mapFilePath);
	~Level();
};

#endif // LEVEL_H