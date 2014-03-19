#include "Jumbotron.h"

using namespace std;
using namespace glm;

Jumbotron::Jumbotron(vec3 offset,GLint thickness, GLint height /* = 300 */, vec3 screen_size/* = vec3 */) 
	:Object(), offset_(offset), height_(height), screen_size_(screen_size), thickness_(thickness)
{

}

bool Jumbotron::Initialize() {
	// Material information
	Ka_ = DEFAULT_JUMBOTRON_KA;
	Kd_ = DEFAULT_JUMBOTRON_KD;
	Ks_ = DEFAULT_JUMBOTRON_KS;
	shininess_ = DEFAULT_JUMBOTRON_SHININESS;

	vec3 a1 = vec3(offset_.x, offset_.y+height_, offset_.z-thickness_/2);
	vec3 b1 = vec3(offset_.x, offset_.y+height_, offset_.z+thickness_/2);
	vec3 c1 = vec3(offset_.x, offset_.y, offset_.z+thickness_/2);
	vec3 d1 = vec3(offset_.x, offset_.y, offset_.z-thickness_/2);

	vec3 a2 = vec3(offset_.x+thickness_, offset_.y+height_, offset_.z-thickness_/2);
	vec3 b2 = vec3(offset_.x+thickness_, offset_.y+height_, offset_.z+thickness_/2);
	vec3 c2 = vec3(offset_.x+thickness_, offset_.y, offset_.z+thickness_/2);
	vec3 d2 = vec3(offset_.x+thickness_, offset_.y, offset_.z-thickness_/2);
	
	add_rectangle_vertices(a1, b1, c1, d1, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(b1, b2, c2, c1, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(b2, a2, d2, c2, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(a2, a1, d1, d2, vec3(1,0,0), this->vertices, this->vertex_indices);

	vec3 a3 = vec3(offset_.x, offset_.y+height_+screen_size_.y, offset_.z-screen_size_.x);
	vec3 b3 = vec3(offset_.x, offset_.y+height_+screen_size_.y, offset_.z+screen_size_.x);
	vec3 c3 = vec3(offset_.x, offset_.y+height_, offset_.z+screen_size_.x);
	vec3 d3 = vec3(offset_.x, offset_.y+height_, offset_.z-screen_size_.x);

	vec3 a4 = vec3(offset_.x+screen_size_.z, offset_.y+height_+screen_size_.y, offset_.z-screen_size_.x);
	vec3 b4 = vec3(offset_.x+screen_size_.z, offset_.y+height_+screen_size_.y, offset_.z+screen_size_.x);
	vec3 c4 = vec3(offset_.x+screen_size_.z, offset_.y+height_, offset_.z+screen_size_.x);
	vec3 d4 = vec3(offset_.x+screen_size_.z, offset_.y+height_, offset_.z-screen_size_.x);

	//add_rectangle_vertices(a3, b3, c3, d3, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(b3, b4, c4, c3, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(b4, a4, d4, c4, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(a4, a3, d3, d4, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(a3, a4, b4, b3, vec3(1,0,0), this->vertices, this->vertex_indices);
	add_rectangle_vertices(d3, c3, c4, d4, vec3(1,0,0), this->vertices, this->vertex_indices);


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

void Jumbotron::TakeDown()
{
	super::TakeDown();
}

void Jumbotron::Draw(
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


