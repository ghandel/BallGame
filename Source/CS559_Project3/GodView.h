#pragma once 
#include "Shader.h"
#include "object.h"
#include "Light.h"
#include "BallController.h"
// This module is responsible for drawing the god view

class GodView : public Object
{
public:
	GodView(GLfloat top=0.97, GLfloat left=0.65, GLfloat bottom=0.65, GLfloat right=0.97);
	virtual bool Initialize(BallController * ballcontroller  = NULL);
	void UpdateBallStatus(BallController & ballcontroller);
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
	GLfloat top_;
	GLfloat left_;
	GLfloat right_;
	GLfloat bottom_;
};