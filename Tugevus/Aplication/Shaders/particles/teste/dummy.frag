#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 Color ;

layout (location = 1) in vec2 inUV;

layout (location = 0 ) out vec4 position ;
layout (location = 1 ) out vec4 normal ;
layout (location = 2 ) out vec4 diffuse ;
layout (location = 3 ) out vec4 material;


layout(set = 1, binding = 0) uniform sampler2D colorTexture;



void main() {
 	vec3 col = texture(colorTexture,vec2(gl_PointCoord.x,gl_PointCoord.y)).rgb;

    
diffuse = vec4(col, Color.x);
 position = vec4(0);material = vec4(0); normal = vec4(0);
}