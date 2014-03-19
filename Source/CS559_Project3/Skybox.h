#pragma once 
#include "SkyboxFacet.h"
#include "vertexattributes.h"
#include "FreeImage.h"
#include <map>
#include <string>
// This module is responsible for drawing the ground pile

/*
class Skybox
{
public:
	Skybox(GLint length = 5280);
	bool SetUp(GLuint & texture_id);
	void Draw();
private:
	GLuint shader_program;
};
*/


class Skybox
{
public:
	// Pile();
	Skybox(GLint length = 5280);
	bool Initialize(GLuint & texture_id);
	void setupCubeMap(GLuint &texture_id, const std::map<std::string, std::string> & image_files);
	void Draw(
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
private:
	// Model data members
	GLuint textid_;
	glm::vec3 Ka_;
	glm::vec3 Kd_;
	glm::vec3 Ks_;
	GLfloat shininess_;
	GLint length_;
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;

	std::vector<VertexAttributes> vertices;
	std::vector<GLuint> vertex_indices;
};
