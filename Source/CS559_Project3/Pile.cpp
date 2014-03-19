#include "Pile.h"

using namespace std;
using namespace glm;

Pile::Pile(
	GLint rows /* = 66 */, 
	GLint columns /* = 66 */, 
	GLint rows_interval /* = 80 */, 
	GLint columns_interval /* = 80 */
):Object(), rows_(rows), columns_(columns), rows_interval_(rows_interval), columns_interval_(columns_interval)
{

}

bool Pile::Initialize() {
	// Material information
	Ka_ = DEFAULT_PILE_KA;
	Kd_ = DEFAULT_PILE_KD;
	Ks_ = DEFAULT_PILE_KS;
	shininess_ = DEFAULT_PILE_SHININESS;

	// Prepare all the vertices as inputs to the shaders
	GLint start_row_index = -rows_ / 2;
	GLint end_row_index = rows_ + start_row_index;
	GLint start_column_index = -columns_ / 2;
	GLint end_column_index = columns_ + start_column_index;	
	GLuint square_offset = 0;

	for (GLint i = start_row_index; i < end_row_index; i ++) {
		for (GLint j = start_column_index; j < end_column_index; j ++) {
			// Each i,j combination represents a square, which is consisted of 2 triangles
			GLfloat top = (GLfloat)i*rows_interval_;
			GLfloat left = (GLfloat)j*columns_interval_;
			GLfloat bottom = (GLfloat)(i+1)*rows_interval_;
			GLfloat right = (GLfloat)(j+1)*columns_interval_;

			vec3 color;
			if((i+j)%2 == 0) {
				color = vec3(1.0,1.0,1.0);
			} else {
				color = vec3(0,0,0);
			}

			// Each vertex attribute is composed of geometry, color, normal and texture coordinate.
			// Top left corner
			this->vertices.push_back(VertexAttributes(vec3(top, 0.0f, left), color, vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)));
			// Top right corner
			this->vertices.push_back(VertexAttributes(vec3(top, 0.0f, right), color, vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)));
			// Bottom left corner
			this->vertices.push_back(VertexAttributes(vec3(bottom, 0.0f, left), color, vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)));
			// Bottom right corner
			this->vertices.push_back(VertexAttributes(vec3(bottom, 0.0f, right), color, vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)));
			
			// Push in the indices
			// Triangle 1
			this->vertex_indices.push_back(square_offset);
			this->vertex_indices.push_back(square_offset+1);
			this->vertex_indices.push_back(square_offset+2);
			// Triangle 2
			this->vertex_indices.push_back(square_offset+2);
			this->vertex_indices.push_back(square_offset+3);
			this->vertex_indices.push_back(square_offset+1);

			square_offset += 4;
		}
	}

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

void Pile::TakeDown()
{
	super::TakeDown();
}

void Pile::Draw(
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


