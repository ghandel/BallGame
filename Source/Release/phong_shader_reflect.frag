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

smooth in vec3 norm;
smooth in vec4 position;
smooth in vec3 color;
smooth in vec2 texture_coord;
smooth in vec3 obj_position;


vec4 phongModel( vec4 position, vec3 norm )
{
  vec3 s = normalize(vec3(modelview_matrix*vec4(Lp,1.0)) - vec3(position));
  vec3 v = normalize(-position.xyz);
  vec3 r = reflect( -s, norm );
  vec3 ambient = La * Ka;
  float sDotN = max( dot(s,norm), 0.0 );
  vec3 diffuse = Ld * Kd * sDotN;
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 )
      spec = Ls * Ks * pow( max( dot(r,v), 0.0 ), shine );
  float attenuation = 1;
  if (1 == with_attenuation) {
		vec3 diff = (obj_position-Lp);
		float dist2 = dot(diff, diff);
		float dist = sqrt(dist2);
		attenuation = 1/(1+0.002*dist+0.00002*dist2);
	} else
		attenuation = 1;

  if (with_texture == 1)
      return vec4((ambient+attenuation*diffuse)*vec3(texture2D(texture, texture_coord)) + attenuation*spec, 1.0);
  else
	  return vec4((ambient+attenuation*diffuse)*color + attenuation*spec, 1.0);
}

void main()
{
	FragColor = 0.5*phongModel( position, norm );
}
