#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 texture_coordinate;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

smooth out vec3 norm;
smooth out vec4 position;
smooth out vec3 color;
smooth out vec2 texture_coord;
smooth out vec3 obj_position;


void main()
{
	// Get the position and normal in eye space
	norm = normalize( normal_matrix * vertex_normal);
	position = modelview_matrix * vec4(vertex_position,1.0);
	color = vertex_color;
	texture_coord = texture_coordinate;
	obj_position = vertex_position;

	gl_Position = mvp * vec4(vertex_position,1.0);
}


