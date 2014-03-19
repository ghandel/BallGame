#pragma once

#pragma once

#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
#include <ft2build.h>
#include <freetype/freetype.h>
// This module is responsible for drawing the god view

class Timer : public Object
{
public:
	Timer();
	virtual bool Initialize();
	void UpdateStatus(const char * text);
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
	void renderTextToTexture(const char * text);
	FT_Face face;
	FT_GlyphSlot g;
	GLfloat left;

	vec3 position_;
	vec3 direction_;
	GLfloat font_size;
private:
	typedef Object super;

	// Model data members

	
};