#pragma once

#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
#include <ft2build.h>
#include <freetype/freetype.h>
// This module is responsible for drawing the god view

class Status : public Object
{
public:
	Status(GLfloat top = 0.87, GLfloat bottom = 0.82);
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
private:
	typedef Object super;

	// Model data members
	GLfloat top_;
	GLfloat bottom_;
	
	
};