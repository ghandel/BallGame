#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
#include "util.h"
// This module is responsible for drawing the ground pile

// Default material property
const vec3 DEFAULT_WALL_KA = vec3(0.5, 0.5, 0.5);
const vec3 DEFAULT_WALL_KD = vec3(0.2, 0.2, 0.2);
const vec3 DEFAULT_WALL_KS = vec3(0.3, 0.3, 0.3);
const GLfloat DEFAULT_WALL_SHININESS = 7.0;

class ArenaWall : public Object
{
public:
	// Pile();
	ArenaWall(GLint width=2800*2, GLint height=280, GLint thickness=160);
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
	GLint width_;
	GLint height_;
	GLint thickness_;
};