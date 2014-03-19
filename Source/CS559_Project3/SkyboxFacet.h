#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
// This module is responsible for drawing the ground pile

// Default material property
const vec3 DEFAULT_SKYBOX_FACET_KA = vec3(1, 1, 1);
const vec3 DEFAULT_SKYBOX_FACET_KD = vec3(0.3, 0.3, 0.3);
const vec3 DEFAULT_SKYBOX_FACET_KS = vec3(0.4, 0.4, 0.4);
const GLfloat DEFAULT_SKYBOX_FACET_SHININESS = 37.0;

enum SkybooxFacetType {BOX_FRONT, BOX_BACK, BOX_TOP, BOX_DOWN, BOX_LEFT, BOX_RIGHT, BOX_NONE};

class SkyboxFacet : public Object
{
public:
	// Pile();
	SkyboxFacet(SkybooxFacetType facet_type = BOX_NONE, GLint length = 5280);
	virtual bool Initialize();
	virtual void Draw(
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
	typedef Object super;

	// Model data members
	SkybooxFacetType facet_type_;
	GLint length_;
};