#include "camera.hpp"

Camera::Camera() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->lookAtV = glm::vec3(0.0, 0.0, 1.0);
	this->forward = this->lookAtV;
	this->up = glm::vec3(0.0, 1.0, 0.0);
	this->right = glm::vec3(1.0, 0.0, 0.0);
	this->velocity = glm::vec3(0.0, 0.0, 0.0);
	this->acceleration = glm::vec3(0.0, 0.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
}

Camera::Camera(glm::vec3* look) {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->lookAtV = glm::vec3(*look);
	this->forward = this->lookAtV;
	this->up = glm::vec3(0.0, 0.0, 0.0);
	this->right = glm::cross(this->forward, this->up);
	this->velocity = glm::vec3(0.0, 0.0, 0.0);
	this->acceleration = glm::vec3(0.0, 0.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
}

Camera::Camera(glm::vec3* pos, glm::vec3* look) {
	this->position = *pos;
	this->lookAtV = glm::vec3(*look);
	this->forward = this->lookAtV;
	this->up = glm::vec3(0.0, 1.0, 0.0);
	this->right = glm::vec3(1.0, 0.0, 0.0);
	this->velocity = glm::vec3(0.0, 0.0, 0.0);
	this->acceleration = glm::vec3(0.0, 0.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
}

Camera::~Camera() {}

void Camera::updateLookAt() {
	glm::vec3 look = glm::vec3(
		this->finalLookAt.x - this->lookAtV.x,
		this->finalLookAt.y - this->lookAtV.y,
		this->finalLookAt.z - this->lookAtV.z
	);
	this->lookAtVelocity = look * 0.5f;
}

void Camera::updateMoveTo() {
	glm::vec3 pos = glm::vec3(
		this->finalPosition.x - this->position.x,
		this->finalPosition.y - this->position.y,
		this->finalPosition.z - this->position.z
	);
	this->velocity = pos * 0.5f;
}

void Camera::rotatePitch(float rad) {
	float sine = sinf(rad);
	float cosine = sinf(rad);
	this->up.y = cosine * this->up.length();
	this->up.z = sine * this->up.length();
	this->forward.y = -sine * this->forward.length();
	this->forward.z = cosine * this->forward.length();
}

void Camera::rotateYaw(float rad) {
	float sine = sinf(rad);
	float cosine = sinf(rad);
	this->right.x = cosine * this->right.length();
	this->right.z = sine * this->right.length();
	this->forward.x = -sine * this->forward.length();
	this->forward.z = cosine * this->forward.length();
}

void Camera::rotatePoll(float rad) {
	float sine = sinf(rad);
	float cosine = sinf(rad);
	this->right.x = cosine * this->right.length();
	this->right.y = sine * this->right.length();
	this->forward.x = -sine * this->forward.length();
	this->forward.y = cosine * this->forward.length();
}

void Camera::lookAtNow(glm::vec3 newPosition) {
	this->lookAtV = newPosition;
}

void Camera::moveToNow(float x, float y, float z) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Camera::moveToNow(glm::vec3 newPosition) {
	this->position = newPosition;
}

void Camera::lookAt(glm::vec3 newPosition) {
	this->initLookAt = this->lookAtV;
	this->finalLookAt = newPosition;
	this->lookAtAccel = -this->lookAtV * 0.25f;
	updateLookAt();
}

void Camera::moveTo(glm::vec3 newPosition) {
	this->initPosition = this->position;
	this->finalPosition = newPosition;
	this->acceleration = this->position * 0.25f;
	updateMoveTo();
}

void Camera::animate(float deltaTime) {
	this->prevPosition = this->position;
	if ((this->yaw >= 360.0f) || (this->yaw <= -360.0f)) this->yaw = 0.0f;
	if (this->pitch > PITCH_CAP) this->pitch = PITCH_CAP;
	if (this->pitch < -PITCH_CAP) this->pitch = -PITCH_CAP;

	float cosYaw = (float)cos(glm::radians(this->yaw));
	float sinYaw = (float)sin(glm::radians(this->yaw));
	float sinPitch = (float)sin(glm::radians(this->pitch));

	float speed = this->velocity.z * deltaTime;
	float strafeSpeed = this->velocity.x * deltaTime;

	if (speed > SPEED_CAP) speed = SPEED_CAP;
	if (strafeSpeed > SPEED_CAP) strafeSpeed = SPEED_CAP;
	if (speed < -SPEED_CAP) speed = -SPEED_CAP;
	if (strafeSpeed < -SPEED_CAP) strafeSpeed = -SPEED_CAP;

	if (this->velocity.length() > 0.0f) this->acceleration = -this->velocity * 1.5f;
	this->velocity += this->acceleration * deltaTime;

	this->position.x += ((float)(cos(glm::radians(this->yaw + 90.0f)))) * strafeSpeed;
	this->position.z += ((float)(sin(glm::radians(this->yaw + 90.0f)))) * strafeSpeed;

	this->position.x += cosYaw * speed;
	this->position.z += sinYaw * speed;

	this->position.y += sinPitch * speed;

	this->lookAtV.x = (float)(position.x + cosYaw);
	this->lookAtV.y = (float)(position.y + sinPitch);
	this->lookAtV.z = (float)(position.z + sinYaw);

	gluLookAt(
		this->position.x,
		this->position.y,
		this->position.z,
		this->lookAtV.x,
		this->lookAtV.y,
		this->lookAtV.z,
		0.0,
		1.0,
		0.0
	);
}