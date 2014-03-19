#pragma once

#pragma once

#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
#include "util.h"
// This module is responsible for drawing the jumbotron

// Default material property
const vec3 DEFAULT_JUMBOTRONSCREEN_KA = vec3(0.85, 0.85, 0.85);
const vec3 DEFAULT_JUMBOTRONSCREEN_KD = vec3(0.2, 0.2, 0.2);
const vec3 DEFAULT_JUMBOTRONSCREEN_KS = vec3(0.4, 0.4, 0.4);
const GLfloat DEFAULT_JUMBOTRONSCREEN_SHININESS = 17.0;

class JumbotronScreen : public Object
{
public:;
	JumbotronScreen(vec3 offset = vec3(0,460,0), vec2 screen_size = vec2(1200,600));
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
	vec2 screen_size_;
};