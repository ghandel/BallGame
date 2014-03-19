#include "Skybox.h"


Skybox::Skybox(GLint length)
	:length_(length)
{
}

bool Skybox::Initialize(GLuint & texture_id){
	// Initialize the vertices and vertex indices
	GLint half_length = length_/2;

	this->vertices.push_back(VertexAttributes(vec3(-1,1,1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(-1,0,1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(1,0,1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(1,1,1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(-1,1,-1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(-1,0,-1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(1,0,-1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	this->vertices.push_back(VertexAttributes(vec3(1,1,-1), vec3(0,0,0), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));

	this->vertex_indices.push_back(0);this->vertex_indices.push_back(1);
	this->vertex_indices.push_back(2);this->vertex_indices.push_back(3);

	this->vertex_indices.push_back(3);this->vertex_indices.push_back(2);
	this->vertex_indices.push_back(6);this->vertex_indices.push_back(7);

	this->vertex_indices.push_back(7);this->vertex_indices.push_back(6);
	this->vertex_indices.push_back(5);this->vertex_indices.push_back(4);

	this->vertex_indices.push_back(4);this->vertex_indices.push_back(5);
	this->vertex_indices.push_back(1);this->vertex_indices.push_back(0);

	this->vertex_indices.push_back(0);this->vertex_indices.push_back(3);
	this->vertex_indices.push_back(7);this->vertex_indices.push_back(4);

	// We don't display the bottom of the skybox
	//this->vertex_indices.push_back(1);this->vertex_indices.push_back(2);
	//this->vertex_indices.push_back(6);this->vertex_indices.push_back(5);

	textid_ = texture_id;

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
	
	return true;
}

void Skybox::TakeDown()
{

}

void Skybox::Draw( 
	Shader & shader, 
	const glm::mat4& projection, 
	glm::mat4 modelview, 
	const glm::ivec2 & size, 
	const Light & light , 
	GLint with_texture, 
	GLint with_attenuation, 
	const float time /* = 0 */ 
) {
	shader.Use();
	mat4 mvp = projection * modelview;
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
	glDrawElements(GL_QUADS , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
#endif

	glUseProgram(0);
	glBindVertexArray(0);
}

