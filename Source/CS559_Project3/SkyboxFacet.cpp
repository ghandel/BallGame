#include "SkyboxFacet.h"

using namespace std;
using namespace glm;


SkyboxFacet::SkyboxFacet(SkybooxFacetType facet_type /* = BOX_NONE */, GLint length /* = 5280 */) 
	:Object(), facet_type_(facet_type), length_(length)
{

}

bool SkyboxFacet::Initialize() {
	// Material information
	Ka_ = DEFAULT_SKYBOX_FACET_KA;
	Kd_ = DEFAULT_SKYBOX_FACET_KD;
	Ks_ = DEFAULT_SKYBOX_FACET_KS;
	shininess_ = DEFAULT_SKYBOX_FACET_SHININESS;

	assert(BOX_NONE != facet_type_);

	vec3 normal_vector;
	// a b c d are the four points that form the facet of the skybox
	vec3 a, b, c, d;
	switch (facet_type_) {
	case BOX_TOP:
		normal_vector = vec3(0,-1,0);
		a = vec3(-length_/2,length_,-length_/2); b = vec3(-length_/2,length_,length_/2);
		c = vec3(length_/2,length_,-length_/2);  d = vec3(length_/2,length_,length_/2);
		break;
	case BOX_DOWN:
		normal_vector = vec3(0,1,0);
		a = vec3(length_/2,0,-length_/2); b = vec3(length_/2,0,length_/2);
		c = vec3(-length_/2,0,-length_/2); d = vec3(-length_/2,0,length_/2);
		break;
	case BOX_FRONT:
		normal_vector = vec3(0,0,-1);
		a = vec3(length_/2,length_,length_/2); b = vec3(-length_/2,length_,length_/2);
		c = vec3(length_/2,0,length_/2); d = vec3(-length_/2, 0,length_/2);
		break;
	case BOX_BACK:
		normal_vector = vec3(0,0,1);
		a = vec3(-length_/2,length_,-length_/2); b = vec3(length_/2,length_,-length_/2);
		c = vec3(-length_/2,0,-length_/2);       d = vec3(length_/2,0,-length_/2);
		break;
	case BOX_LEFT:
		normal_vector = vec3(1,0,0);
		a = vec3(-length_/2,length_,length_/2); b = vec3(-length_/2, length_, -length_/2);
		c = vec3(-length_/2,0,length_/2);       d = vec3(-length_/2, 0, -length_/2);
		break;
	case BOX_RIGHT:
		normal_vector = vec3(-1,0,0);
		a = vec3(length_/2,length_,-length_/2); b = vec3(length_/2,length_,length_/2);
		c = vec3(length_/2,0,-length_/2);       d = vec3(length_/2,0,length_/2);
		break;
	default:
		std::cerr << "Invalid facet type \n";
		break;
	}
	
	// Add the vertices and vertex indices
	vec3 color = vec3(0,0,1);
	this->vertices.push_back(VertexAttributes(a, color, vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
	// Top right corner
	this->vertices.push_back(VertexAttributes(b, color, vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)));
	// Bottom left corner
	this->vertices.push_back(VertexAttributes(c, color, vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)));
	// Bottom right corner
	this->vertices.push_back(VertexAttributes(d, color, vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)));
	
	// Triangle 1
	this->vertex_indices.push_back(0);
	this->vertex_indices.push_back(1);
	this->vertex_indices.push_back(2);
	// Triangle 2
	this->vertex_indices.push_back(2);
	this->vertex_indices.push_back(3);
	this->vertex_indices.push_back(1);

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

void SkyboxFacet::TakeDown()
{
	super::TakeDown();
}

void SkyboxFacet::Draw(
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


