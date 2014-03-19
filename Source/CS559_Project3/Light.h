#pragma once
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// Default light properties
const vec3 DEFAULT_LA = vec3(1.0, 1.0, 1.0);
const vec3 DEFAULT_LD = vec3(1.0, 1.0, 1.0);
const vec3 DEFAULT_LS = vec3(1.0, 1.0, 1.0);
const vec3 DEFAULT_LIGHT_POSITION = vec3(0.0, 700, 0);

class Light {
public:
	Light(vec3 La = DEFAULT_LA, vec3 Ld = DEFAULT_LD, vec3 Ls = DEFAULT_LS, vec3 lp = DEFAULT_LIGHT_POSITION);
	~Light();
	const GLfloat * getLaValuePtr()const;
	const GLfloat * getLdValuePtr()const;
	const GLfloat * getLsValuePtr()const;
	const GLfloat * getLightPositionValuePtr()const;
private:
	vec3 La_;
	vec3 Ld_;
	vec3 Ls_;
	vec3 Light_Position_;
};