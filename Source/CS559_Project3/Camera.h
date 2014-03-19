#pragma once
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// This module provides abstraction for the camera object

class Camera {
public:
	// The camera's position is measured in a spherical coordinate system
	Camera(GLint yaw = 135, GLint pitch = 20, GLfloat radius = 2000.0f, GLfloat fov = 80.0f);
	~Camera();

	// Get the camera position in rectangular coordinate system(x,y,z) relative to the world origin
	glm::vec3 getCameraPosition()const;

	GLfloat getFov()const;

	// Called by callback functions to deal with up/down/left/right key event
	void addYaw();
	void minusYaw();
	void addPitch();
	void minusPitch();
	void addFov();
	void minusFov();
private:
	// These parameters for the positioning of the camera
	GLint yaw_;
	GLint pitch_;
	GLfloat radius_;
	GLfloat fov_;

};