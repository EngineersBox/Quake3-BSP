#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
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
	void updateLookAt();
	void updateMoveTo();
public:
	glm::vec3 position;
	glm::vec3 prevPosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 lookAt;
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;

	float yaw;
	float pitch;

	int screenWidth;
	int screenHeight;
	int centreX;
	int centreY;

	Camera();
	Camera(int width, int height) {}
	Camera(glm::vec3* look);
	Camera(glm::vec3* pos, glm::vec3* look);
	~Camera();

	//void lookAt(Element* element);
	//void lookAtNow(Element* element);
	//void moveTo(Element* element);
	//void moveToNow(Element* element);
	void moveToNow(float x, float y, float z);
	void rotateYaw(float rad);
	void rotatePitch(float rad);
	void rotatePoll(float rad);
	void animate(float deltaTime);
};

#endif // CAMERA_H