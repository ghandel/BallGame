#include "GodView.h"

using namespace std;
using namespace glm;

GodView::GodView(GLfloat top/* =0.85 */, GLfloat left/* =0.85 */, GLfloat bottom/* =0.75 */, GLfloat right/* =0.95 */) 
	:Object(), top_(top), left_(left), bottom_(bottom), right_(right)
{

}

bool GodView::Initialize(BallController * ballcontroller) {

	// First draw the area
	this->vertices.push_back(VertexAttributes(vec3(0,0,0), vec3(0.1,0.1,0.1), vec3(1,0,0), vec2(left_, top_)));
	this->vertices.push_back(VertexAttributes(vec3(0,0,0), vec3(0.1,0.1,0.1), vec3(1,0,0), vec2(right_, top_)));
	this->vertices.push_back(VertexAttributes(vec3(0,0,0), vec3(0.1,0.1,0.1), vec3(1,0,0), vec2(right_, bottom_)));
	this->vertices.push_back(VertexAttributes(vec3(0,0,0), vec3(0.1,0.1,0.1), vec3(1,0,0), vec2(left_, bottom_)));

	


	int wall_width = 5600;
	for (unsigned int i = 0; i < ballcontroller->balls.size(); i ++) {
		float x = ballcontroller->balls[i].position.x;
		float y = ballcontroller->balls[i].position.y;
		float top = y+50; float bottom = y -50;
		float left = x-50; float right = x+50;
		top = (float)(top/wall_width + 0.5)*(top_-bottom_)+ bottom_;
		bottom = (float)(bottom/wall_width + 0.5)*(top_-bottom_)+ bottom_;
		left = (float)(left/wall_width + 0.5) * (right_-left_) + left_;
		right =(float)(right/wall_width + 0.5) * (right_ - left_) + left_;
		
		vec3 color;
		if (ballcontroller->balls[i].status == 0) {
			color = vec3(1,0,0);
		} else {
			color = vec3(0,1,0);
		}
		this->vertices.push_back(VertexAttributes(vec3(0,0,0), color, vec3(1,0,0), vec2(left, top)));
		this->vertices.push_back(VertexAttributes(vec3(0,0,0), color, vec3(1,0,0), vec2(right, top)));
		this->vertices.push_back(VertexAttributes(vec3(0,0,0), color, vec3(1,0,0), vec2(right, bottom)));
		this->vertices.push_back(VertexAttributes(vec3(0,0,0), color, vec3(1,0,0), vec2(left, bottom)));
		this->vertex_indices.push_back(4*(i+1)+0); this->vertex_indices.push_back(4*(i+1)+1);
		this->vertex_indices.push_back(4*(i+1)+2);

		this->vertex_indices.push_back(4*(i+1)+2); this->vertex_indices.push_back(4*(i+1)+0);
		this->vertex_indices.push_back(4*(i+1)+3);


	}

	this->vertex_indices.push_back(0); this->vertex_indices.push_back(1);
	this->vertex_indices.push_back(2);

	this->vertex_indices.push_back(2); this->vertex_indices.push_back(0);
	this->vertex_indices.push_back(3);

	//	The vertex array serves as a handle for the whole bundle.
	glGenVertexArrays(1, &this->vertex_array_handle);
	glBindVertexArray(this->vertex_array_handle);

	//	The vertex buffer serves as a container for the memory to be defined.
	glGenBuffers(1, &this->vertex_coordinate_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0], GL_DYNAMIC_DRAW);
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

void GodView::TakeDown()
{
	super::TakeDown();
}

void GodView::Draw(
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


void GodView::UpdateBallStatus(BallController & ballcontroller) {
	//this->TakeDown();
	//this->Initialize(&ballcontroller);

	
	for (unsigned int i = 0; i < ballcontroller.balls.size(); i ++)
	{
		vec3 color;
		if (ballcontroller.balls[i].status == 0) {
			color = vec3(1,0,0);
		} else {
			color = vec3(0,1,0);
		}
		this->vertices[4*(i+1)].color = color;
		this->vertices[4*(i+1)+1].color = color;
		this->vertices[4*(i+1)+2].color = color;
		this->vertices[4*(i+1)+3].color = color;
	}
	glBindVertexArray(this->vertex_array_handle);
	//	The vertex buffer serves as a container for the memory to be defined.
	//glGenBuffers(1, &this->vertex_coordinate_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0]);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
}