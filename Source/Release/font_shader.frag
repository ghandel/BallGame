#version 400

layout (location = 0) out vec4 FragColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shine;
uniform int with_texture;
uniform int with_attenuation;

uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform vec3 Lp;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;
uniform sampler2D texture;

smooth in vec3 color;
smooth in vec2 texture_coord;



void main()
{
	FragColor = vec4(1,1,1,texture2D(texture, texture_coord).a);
	//FragColor = vec4(1,1,1,0.5);

}
