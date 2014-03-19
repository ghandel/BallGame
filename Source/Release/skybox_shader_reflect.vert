#version 400
 
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 texture_coordinate;

out vec3 texCoord;
uniform mat4 mvp;
 
void main() {
    //gl_Position = mvp * vec4(2800*(vertex_position+vec3(0,1,0)), 1.0);
	//gl_Position.z = gl_Position.w -0.00001;
	gl_Position = mvp * vec4(vertex_position, 1.0);

    texCoord = vertex_position;
}