#include "ArenaWall.h"

using namespace std;
using namespace glm;

ArenaWall::ArenaWall(GLint width/* =2800 */, GLint height/* =400 */, GLint thickness/* =160 */)
	:Object(), width_(width), height_(height), thickness_(thickness) {

}


bool ArenaWall::Initialize() {
	// Material information
	Ka_ = DEFAULT_WALL_KA;
	Kd_ = DEFAULT_WALL_KD;
	Ks_ = DEFAULT_WALL_KS;
	shininess_ = DEFAULT_WALL_SHININESS;


	GLint half_width = width_/2;
	vec3 a1p(-half_width, height_, -half_width);
	
	this->vertices.push_back(VertexAttributes(vec3(-half_width, height_, -half_width), vec3(1,0,0),vec3(0,1,0),vec2(0,0)));
	this->vertices.push_back(VertexAttributes(vec3(half_width, height_, -half_width), vec3(1,0,0),vec3(0,1,0),vec2(1,0)));
	this->vertices.push_back(VertexAttributes(vec3(half_width, height_, half_width), vec3(1,0,0),vec3(0,1,0),vec2(1,1)));
	this->vertices.push_back(VertexAttributes(vec3(-half_width, height_, half_width), vec3(1,0,0),vec3(0,1,0),vec2(0,1)));

	GLint half_width_i = half_width-thickness_;
	GLfloat txc_offset = (float)thickness_/half_width;
	this->vertices.push_back(VertexAttributes(vec3(-half_width_i, height_, -half_width_i), vec3(1,0,0),vec3(0,1,0),vec2(txc_offset,txc_offset)));
	this->vertices.push_back(VertexAttributes(vec3(half_width_i, height_, -half_width_i), vec3(1,0,0),vec3(0,1,0),vec2(1-txc_offset,txc_offset)));
	this->vertices.push_back(VertexAttributes(vec3(half_width_i, height_, half_width_i), vec3(1,0,0),vec3(0,1,0),vec2(1-txc_offset,1-txc_offset)));
	this->vertices.push_back(VertexAttributes(vec3(-half_width_i, height_, half_width_i), vec3(1,0,0),vec3(0,1,0),vec2(txc_offset,1-txc_offset)));

	this->vertex_indices.push_back(0); this->vertex_indices.push_back(1); this->vertex_indices.push_back(4);
	this->vertex_indices.push_back(1); this->vertex_indices.push_back(4); this->vertex_indices.push_back(5);
	this->vertex_indices.push_back(1); this->vertex_indices.push_back(2); this->vertex_indices.push_back(5);
	this->vertex_indices.push_back(2); this->vertex_indices.push_back(5); this->vertex_indices.push_back(6);
	this->vertex_indices.push_back(2); this->vertex_indices.push_back(6); this->vertex_indices.push_back(7);
	this->vertex_indices.push_back(2); this->vertex_indices.push_back(3); this->vertex_indices.push_back(7);
	this->vertex_indices.push_back(0); this->vertex_indices.push_back(3); this->vertex_indices.push_back(4);
	this->vertex_indices.push_back(3); this->vertex_indices.push_back(4); this->vertex_indices.push_back(7);

	add_rectangle_vertices(
		vec3(-half_width_i,0,-half_width_i),
		vec3(-half_width_i,0,half_width_i),
		vec3(-half_width_i,height_,half_width_i),
		vec3(-half_width_i,height_,-half_width_i),
		vec3(1,0,0),
		this->vertices,
		this->vertex_indices
    );

	add_rectangle_vertices(
		vec3(half_width_i,0,-half_width_i),
		vec3(-half_width_i,0,-half_width_i),
		vec3(-half_width_i,height_,-half_width_i),
		vec3(half_width,height_,-half_width_i),
		vec3(1,0,0),
		this->vertices,
		this->vertex_indices
	);

	add_rectangle_vertices(
		vec3(half_width_i,0,half_width_i),
		vec3(half_width_i,0,-half_width_i),
		vec3(half_width_i,height_,-half_width_i),
		vec3(half_width_i,height_,half_width_i),
		vec3(1,0,0),
		this->vertices,
		this->vertex_indices
	);

	add_rectangle_vertices(
		vec3(-half_width_i,0,half_width_i),
		vec3(half_width_i,0,half_width_i),
		vec3(half_width_i,height_,half_width_i),
		vec3(-half_width_i,height_,half_width_i),
		vec3(1,0,0),
		this->vertices,
		this->vertex_indices
	);

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

void ArenaWall::TakeDown()
{
	super::TakeDown();
}

void ArenaWall::Draw(
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


