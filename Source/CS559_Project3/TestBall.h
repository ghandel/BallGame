#pragma once 
#include "Shader.h"
#include "object.h"
#include "util.h"
#include "Light.h"
// This module implements a ball, which helps debugging

const vec3 DEFAULT_TEST_BALL_KA = vec3(1.2, 1.2, 1.2);
const vec3 DEFAULT_TEST_BALL_KD = vec3(1.6, 1.6, 1.6);
const vec3 DEFAULT_TEST_BALL_KS = vec3(0.8, 0.8, 0.8);
const GLfloat DEFAULT_TEST_BALL_SHININESS = 40.0;

class TestBall : public Object
{
public:
	TestBall(GLfloat scale = 1.0);
	virtual bool Initialize();
	virtual void Draw(
		Shader & shader, 
		const glm::mat4& projection, 
		glm::mat4 modelview, 
		const glm::ivec2 & size, 
		const Light & light,
		GLint with_texture,
		GLint with_attenuation,
		const float time = 0
		);
	void TakeDown();
private:
	typedef Object super;

	// Model data members
	GLfloat scale_;
};