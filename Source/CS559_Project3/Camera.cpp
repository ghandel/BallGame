#include "Camera.h"
#include <iostream>
Camera::Camera(GLint yaw, GLint pitch /* = 60 */, GLfloat radius /* = 90.0f */, GLfloat fov/* = 40.0f */)
	:yaw_(yaw), pitch_(pitch), radius_(radius), fov_(fov){

}

Camera::~Camera() {

}

glm::vec3 Camera::getCameraPosition()const {
	// The following code converts spherical coordinate system into rectangular coordinate system
	float proj_r = radius_ * glm::cos(glm::radians((float)pitch_));
	float x = proj_r * glm::cos(glm::radians((float)yaw_));
	float z = proj_r * glm::sin(glm::radians((float)yaw_));
	float y = radius_ * glm::sin(glm::radians((float)pitch_));
	return glm::vec3(x, y, z);
}

void Camera::addYaw() {
	
	yaw_ = (yaw_+1)%360;
}

void Camera::minusYaw() {
	yaw_ = (yaw_-1)%360;
}

// The range of pitch is confined to [-89 degree, +89 degree]
void Camera::addPitch(){
	pitch_ = glm::min(pitch_+1,89);
}

void Camera::minusPitch() {
	pitch_ = glm::max(pitch_-1,1);
}

void Camera::addFov() {
	fov_ = (GLfloat)glm::min(120.0, fov_+1.0);
}

void Camera::minusFov() {
	fov_ = (GLfloat)glm::max(10.0, fov_-1.0);
}

GLfloat Camera::getFov()const {
	return fov_;
}