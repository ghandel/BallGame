#pragma once
#include <vector>
#include <assert.h>
#include <gl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertexattributes.h"

using namespace std;
using namespace glm;


/*  Auxiliary functions to draw primitives 
	Each of them takes in certain parameters, and push vertices and vertex indices to the current vector
*/


// Yaw and pitch are both in radians
vec3 get_norm_from_yaw_pitch(GLfloat yaw, GLfloat pitch);

// We assume the either the lower_radius or the upper_radius is larger than 0
void add_cylinder_vertices(
	GLfloat lower_radius,
	GLfloat upper_radius,
	GLfloat height,
	GLfloat offset,
	GLuint slices,
	GLuint stacks,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices,
	bool is_out_ward_norm = true
);

void add_disk_vertices(
	GLfloat inner_radius,
	GLfloat outer_radius,
	GLfloat offset,
	GLuint slices,
	GLuint loops,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices,
	bool is_upward_norm=true
);

/* a b c d are the four corner pointers of the rectangle, their input should follow the 
   order such that the direction of cross product of a-b and d-a should be the direction
   of the normal of this rectangle
*/
void add_rectangle_vertices(
	vec3 a,
	vec3 b,
	vec3 c,
	vec3 d,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices
);

void add_torus_vertices(
	GLfloat inner_radius,
	GLfloat outer_radius,
	GLfloat offset,
	GLuint sides,
	GLuint rings,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices
);

void add_ball_vertices(
	vec3 center,
	GLfloat radius,
	GLuint stacks,
	GLuint slices,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices
);