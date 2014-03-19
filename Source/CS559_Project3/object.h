/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:    Modified by Chunhui Zhu 3/20/2013
*/

#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertexattributes.h"
#include "Shader.h"
#include "Light.h"

class Object
{
public:
	Object();

	virtual void TakeDown();
	virtual bool Initialize();
	virtual void Draw(
		Shader & shader, 
		const glm::mat4 & projection, 
		glm::mat4 modelview, 
		const glm::ivec2 & size, 
		const Light & light, 
		GLint with_texture,
		GLint with_attenuation,
		const float time = 0
	) = 0;
	virtual ~Object();

	// Texture control member
	GLuint textid_;

protected:
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;

	bool GLReturnedError(char * s);
	std::vector<VertexAttributes> vertices;
	std::vector<GLuint> vertex_indices;

	// Material properties
	glm::vec3 Ka_;
	glm::vec3 Kd_;
	glm::vec3 Ks_;
	GLfloat shininess_;

	

private:
	void InternalInitialize();
};
