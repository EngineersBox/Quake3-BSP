#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#endif

#include <glm/glm.hpp>

constexpr float SPEED_CAP = 15.0f;
constexpr float PITCH_CAP = 60.0f;

class Camera {
private:
	glm::vec3 initPosition;
	glm::vec3 finalPosition;
	glm::vec3 initLookAt;
	glm::vec3 finalLookAt;
	glm::vec3 lookAtVelocity;
	glm::vec3 lookAtAccel;
	void updateLookAt();
	void updateMoveTo();
public:
	glm::vec3 position;
	glm::vec3 prevPosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 lookAtV;
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;

	float yaw = 0;
	float pitch = 0;

	int screenWidth = 0;
	int screenHeight = 0;
	int centreX = 0;
	int centreY = 0;

	Camera();
	Camera(int width, int height) {}
	Camera(glm::vec3* look);
	Camera(glm::vec3* pos, glm::vec3* look);
	~Camera();

	void lookAt(glm::vec3 newPosition);
	void lookAtNow(glm::vec3 newPosition);
	void moveTo(glm::vec3 newPosition);
	void moveToNow(glm::vec3 newPosition);
	void moveToNow(float x, float y, float z);
	void rotateYaw(float rad);
	void rotatePitch(float rad);
	void rotatePoll(float rad);
	void animate(float deltaTime);
};

#endif // CAMERA_H