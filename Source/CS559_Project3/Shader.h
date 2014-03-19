#pragma once
/* This module implements the shader function
   This module is adapted from:
     Perry Kivolowitz - University of Wisconsin - Madison 
     Computer Sciences Department

     A sample hello world like program demonstrating modern
     OpenGL techniques. 
*/

#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextureManager.h"

class Shader
{
public:
	Shader();
	void TakeDown();
	void Use();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup();
	void CommonSetup(
		const float time, 
		const GLint * size, 
		const GLfloat * projection, 
		const GLfloat * modelview, 
		const GLfloat * mvp, 
		const GLfloat * nm,
		const GLfloat * ka,
		const GLfloat * kd,
		const GLfloat * ks,
		const GLfloat shiniess,
		const GLfloat * la,
		const GLfloat * ld,
		const GLfloat * ls,
		const GLfloat * light_position,
		const GLint with_texture,
		const GLint with_attenuation,
		const GLuint text_id
	);

	GLuint modelview_matrix_handle;
	GLuint projection_matrix_handle;
	GLuint normal_matrix_handle;
	GLuint mvp_handle;
	GLuint size_handle;
	GLuint time_handle;

	// Material related
	GLuint ka_handle;
	GLuint kd_handle;
	GLuint ks_handle;
	GLuint shininess_handle;

	// Lighting related
	GLuint la_handle;
	GLuint ld_handle;
	GLuint ls_handle;
	GLuint light_position_handle;

	// Texture related
	GLuint texture_handle;
	GLuint with_texture_handle;
	GLuint cube_texture_handle;

	// Attenuation
	GLuint with_attenuation_handle;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	std::stringstream GetShaderLog(GLuint shader_id);

protected:
	bool GLReturnedError(char * s);

private:
	typedef Shader super;
};
