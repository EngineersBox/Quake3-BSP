#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <string>
#include <vector>
#include <spdlog/spdlog.h>

#include "../../resource/loader/QLoader.hpp"
#include "../../resource/material/texture.hpp"
#include "../../view/camera.hpp"
#include "../../view/frustum.hpp"
#include "../element/element.hpp"

class Level : public Element {
private:
	QMapQ3 map;
	std::vector<Material::Texture> lightmaps;
	std::vector<Material::Texture> albedos;

	void convertEdges();
    void bindLightmapAndTexture(int faceIndex);
	void drawTriangleVA(int faceIndex);
	void drawTriangleMesh(int faceIndex);

	// BSP Related
	int findLeaf(const glm::vec3& cameraPos);
	bool clusterVisible(int actual, int leafCluster) const;
	void generateLightmaps();
	void generateAlbedos();

	// Collisions
	int traceType;
	float sphereRadius;
	void checkNode(int nodeIndex, float deltaInitial, float deltaFinal, glm::vec3 pStart, glm::vec3 pFinal);
	void checkBrush(QBrush* brush, glm::vec3 vOrigin, glm::vec3 vFinal);
protected:
	void onAnimate(float deltaTime) override {}
	void onDraw(Camera* camera) override;
	void onCollision(Element* element) override;
public:
	int triangles;
	Frustum frustum;
    float fogColor[4]{0.75f, 0.9f, 1.0f, 1.0f};
    float fogStart = 10.0f;
    float fogEnd = 3000.0f;

	// Collisions
	bool collision;
	float outputDelta;

	glm::vec3 trace(glm::vec3 start, glm::vec3 end);
    [[maybe_unused]] glm::vec3 traceRay(glm::vec3 start, glm::vec3 end);
	glm::vec3 traceSphere(glm::vec3 start, glm::vec3 end, float radius);

	explicit Level(const char* mapFilePath);
	~Level() override;
};

#endif // LEVEL_H