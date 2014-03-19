#include "JumbotronScreen.h"

using namespace std;
using namespace glm;

JumbotronScreen::JumbotronScreen(vec3 offset, vec2 screen_size) 
	:Object(), offset_(offset), screen_size_(screen_size)
{

}

bool JumbotronScreen::Initialize() {
	// Material information
	Ka_ = DEFAULT_JUMBOTRONSCREEN_KA;
	Kd_ = DEFAULT_JUMBOTRONSCREEN_KD;
	Ks_ = DEFAULT_JUMBOTRONSCREEN_KS;
	shininess_ = DEFAULT_JUMBOTRONSCREEN_SHININESS;

	vec3 a = vec3(offset_.x, offset_.y+screen_size_.y, offset_.z-screen_size_.x/2);
	vec3 b = vec3(offset_.x, offset_.y+screen_size_.y, offset_.z+screen_size_.x/2);
	vec3 c = vec3(offset_.x, offset_.y, offset_.z+screen_size_.x/2);
	vec3 d = vec3(offset_.x, offset_.y, offset_.z - screen_size_.x/2);

	this->vertices.push_back(VertexAttributes(a,vec3(1,0,0),vec3(-1,0,0),vec2(0,0.7)));//0/7
	this->vertices.push_back(VertexAttributes(b,vec3(1,0,0),vec3(-1,0,0),vec2(1,0.7)));
	this->vertices.push_back(VertexAttributes(c,vec3(1,0,0),vec3(-1,0,0),vec2(1,0)));
	this->vertices.push_back(VertexAttributes(d,vec3(1,0,0),vec3(-1,0,0),vec2(0,0)));
	

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
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0], GL_STATIC_DRAW);

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

void JumbotronScreen::TakeDown()
{
	super::TakeDown();
}

void JumbotronScreen::Draw(
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


