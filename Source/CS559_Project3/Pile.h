#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
// This module is responsible for drawing the ground pile

// Default material property
const vec3 DEFAULT_PILE_KA = vec3(0.1, 0.1, 0.1);
const vec3 DEFAULT_PILE_KD = vec3(0.3, 0.3, 0.3);
const vec3 DEFAULT_PILE_KS = vec3(0.4, 0.4, 0.4);
const GLfloat DEFAULT_PILE_SHININESS = 37.0;

class Pile : public Object
{
public:
	// Pile();
	Pile(GLint rows = 66, GLint columns = 66, GLint rows_interval = 80, GLint columns_interval = 80);
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
	GLint rows_;
	GLint columns_;
	GLint rows_interval_;
	GLint columns_interval_;
};