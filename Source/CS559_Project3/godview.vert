#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 texture_coordinate;

smooth out vec3 color;


void main()
{
	color = vertex_color;

	gl_Position = vec4(texture_coordinate,0.0, 1.0);
}
