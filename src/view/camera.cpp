#include "camera.hpp"

Camera::Camera() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->lookAtV = glm::vec3(0.0, 0.0, 1.0);
	this->forward = this->lookAtV;
	this->up = glm::vec3(0.0, 1.0, 0.0);
	this->right = glm::vec3(1.0, 0.0, 0.0);
	this->horizontalVelocity = glm::vec3(0.0, 0.0, 0.0);
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
	this->horizontalVelocity = glm::vec3(0.0, 0.0, 0.0);
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
	this->horizontalVelocity = glm::vec3(0.0, 0.0, 0.0);
	this->acceleration = glm::vec3(0.0, 0.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
}

Camera::~Camera() = default;

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
	this->horizontalVelocity = pos * 0.5f;
}

inline float vecLength(glm::vec3 vec) {
    return (float)sqrt((double)(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

void Camera::rotatePitch(float rad) {
	float sine = sinf(rad);
	float cosine = sinf(rad);
	this->up.y = cosine * vecLength(this->up);
	this->up.z = sine * vecLength(up);
	this->forward.y = -sine * vecLength(this->forward);
	this->forward.z = cosine * vecLength(this->forward);
}

void Camera::rotateYaw(float rad) {
	float sine = sinf(rad);
	float cosine = sinf(rad);
	this->right.x = cosine * vecLength(this->right);
	this->right.z = sine * vecLength(this->right);
	this->forward.x = -sine * vecLength(this->forward);
	this->forward.z = cosine * vecLength(this->forward);
}

void Camera::rotatePoll(float rad) {
	float sine = sinf(rad);
	float cosine = sinf(rad);
	this->right.x = cosine * vecLength(this->right);
	this->right.y = sine * vecLength(this->right);
	this->forward.x = -sine * vecLength(this->forward);
	this->forward.y = cosine * vecLength(this->forward);
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

	auto cosYaw = (float)cos(glm::radians(this->yaw));
	auto sinYaw = (float)sin(glm::radians(this->yaw));
	auto sinPitch = (float)sin(glm::radians(this->pitch));

	float speed = this->horizontalVelocity.z * deltaTime;
	float strafeSpeed = this->horizontalVelocity.x * deltaTime;

	if (speed > SPEED_CAP) speed = SPEED_CAP;
	if (strafeSpeed > SPEED_CAP) strafeSpeed = SPEED_CAP;
	if (speed < -SPEED_CAP) speed = -SPEED_CAP;
	if (strafeSpeed < -SPEED_CAP) strafeSpeed = -SPEED_CAP;

	if (vecLength(horizontalVelocity) > 0.0f) this->acceleration = -this->horizontalVelocity * 1.5f;
	this->horizontalVelocity += this->acceleration * deltaTime;

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