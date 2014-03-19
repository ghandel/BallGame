#include "TextManager.h"
#include <iostream>
TextManager::TextManager() {

}

void TextManager::Initialize() {
	FT_Library ft;

	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return;
	}

	

	if(FT_New_Face(ft, "ska_75_marul_ce.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Set up the related texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &this->texture_id);
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextManager::renderTextToTexture(const char * text) {
	const char *p;
	FT_GlyphSlot g = face->glyph;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(p = text; *p; p++) {
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture_id);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
			);

		width = g->bitmap.width;
		height = g->bitmap.rows;

		for (int i = 0; i < g->bitmap.rows; i ++) {
			for (int j = 0; j < g->bitmap.width; j ++) {
				if((int)g->bitmap.buffer[i*(g->bitmap.width)+j]> 128) {
					std::cout << "1";
				} else {
					std::cout << "0";
				}

			}
			std::cout << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		break;
	}

}