#include "Status.h"

using namespace std;
using namespace glm;

Status::Status(GLfloat top /* = -0.85 */, GLfloat bottom /* = -0.97 */)
	:Object(), top_(top), bottom_(bottom) {
		left = (float)-0.97;

}

bool Status::Initialize() {
	FT_Library ft;

	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return false;
	}



	if(FT_New_Face(ft, "ska_75_marul_ce.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Set up the related texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &this->textid_);
	glBindTexture(GL_TEXTURE_2D, this->textid_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	g = face->glyph;

	//renderTextToTexture("T");

	GLfloat right = (float)0.97;
	GLfloat left = 0;
		//right - (float)g->bitmap.width/g->bitmap.rows*(top_-bottom_);

	this->vertices.push_back(VertexAttributes(vec3(left,top_,0), vec3(0,0,1), vec3(0,0,1),vec2(0,0)));
	this->vertices.push_back(VertexAttributes(vec3(right,top_,0), vec3(0,0,1), vec3(0,0,1),vec2(1,0)));
	this->vertices.push_back(VertexAttributes(vec3(right,bottom_,0), vec3(0,0,1), vec3(0,0,1),vec2(1,1)));
	this->vertices.push_back(VertexAttributes(vec3(left,bottom_,0), vec3(0,0,1), vec3(0,0,1),vec2(0,1)));

	this->vertex_indices.push_back(0); this->vertex_indices.push_back(1);
	this->vertex_indices.push_back(2);

	this->vertex_indices.push_back(2);this->vertex_indices.push_back(0);
	this->vertex_indices.push_back(3);
	//	The vertex array serves as a handle for the whole bundle.
	glGenVertexArrays(1, &this->vertex_array_handle);
	glBindVertexArray(this->vertex_array_handle);

	//	The vertex buffer serves as a container for the memory to be defined.
	glGenBuffers(1, &this->vertex_coordinate_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3) * 3));

	//	Each of the attributes to be used must be enabled.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Triangle::Initialize - on exit"))
		return false;

	return true;

}


void Status::renderTextToTexture(const char * text) {
	const char *p=text;
	FT_GlyphSlot g = face->glyph;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//for(p = text; *p; p++) {
		FT_Load_Char(face, *p, FT_LOAD_RENDER);
			//continue;
		
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
		
		//break;
	//}

}


void Status::TakeDown()
{
	super::TakeDown();
}

void Status::UpdateStatus(const char * text){
	renderTextToTexture(text);

	//GLfloat left = -0.95;
	GLfloat right = left + (float)g->bitmap.width/g->bitmap.rows*(top_-bottom_);

	this->vertices[0].position = vec3(left,top_,0);
	this->vertices[1].position = vec3(right,top_,0);
	this->vertices[2].position = vec3(right,bottom_,0);	
	this->vertices[3].position = vec3(left,bottom_,0);

	glBindVertexArray(this->vertex_array_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0]);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	left = (float)(right + 0.01);

}

void Status::Draw(
	Shader & shader, 
	const mat4 & projection, 
	mat4 modelview, 
	const ivec2 & size, 
	const Light & light, 
	GLint with_texture,
	GLint with_attenuation,
	const float time
	) {
		if (this->GLReturnedError("Pile::Draw - on entry"))
			return;

		mat4 mvp = projection * modelview;

		shader.Use();
		shader.CommonSetup(
			time, 
			value_ptr(size), 
			value_ptr(projection), 
			value_ptr(modelview), 
			value_ptr(mvp), 
			value_ptr(mat3(transpose(inverse(modelview)))),
			value_ptr(this->Ka_),
			value_ptr(this->Kd_),
			value_ptr(this->Ks_),
			this->shininess_,
			light.getLaValuePtr(),
			light.getLdValuePtr(),
			light.getLsValuePtr(),
			light.getLightPositionValuePtr(),
			with_texture,
			with_attenuation,
			this->textid_
			);

		glBindVertexArray(this->vertex_array_handle);
#ifdef DRAW_POINTS
		glDrawArrays(GL_POINTS, 0, this->vertex_indices.size());
#else
		glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
#endif

		glUseProgram(0);
		glBindVertexArray(0);

		if (this->GLReturnedError("Pile::Draw - on exit"))
			return;
}


