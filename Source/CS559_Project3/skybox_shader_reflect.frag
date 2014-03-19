#version 400
 
in vec3 texCoord;
out vec4 fragColor;
uniform samplerCube cubemap;
 
void main (void) {
    fragColor = 0.50*texture(cubemap, texCoord);
}
