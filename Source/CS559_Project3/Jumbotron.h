#pragma once

#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
#include "util.h"
// This module is responsible for drawing the jumbotron

// Default material property
const vec3 DEFAULT_JUMBOTRON_KA = vec3(0.25, 0.25, 0.25);
const vec3 DEFAULT_JUMBOTRON_KD = vec3(0.2, 0.2, 0.2);
const vec3 DEFAULT_JUMBOTRON_KS = vec3(0.4, 0.4, 0.4);
const GLfloat DEFAULT_JUMBOTRON_SHININESS = 17.0;

class Jumbotron : public Object
{
public:
	// Pile();
	Jumbotron(vec3 offset = vec3(0, 280, 0), GLint thickness = 30, GLint height = 180, vec3 screen_size= vec3(600,600,30));
	virtual bool Initialize();
	virtual void Draw(
		Shader & shader, 
		const glm::mat4& projection, 
		glm::mat4 modelview, 
		const glm::ivec2 & size, 
		const Light & light , 
		GLint with_texture,
		GLint with_attenuation,
		const float time = 0
		);
	void TakeDown();
private:
	typedef Object super;

	// Model data members
	vec3 offset_;
	GLint height_;
	GLint thickness_;
	vec3 screen_size_;
};