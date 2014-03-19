#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include <freetype/freetype.h>
// #include FT_FREETYPE_H

class TextManager{
public:
	TextManager();
	void Initialize();
	void renderTextToTexture(const char * text);
	GLuint texture_id;
	FT_Face face;

	int width;
	int height;
private:
	
};