#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 TexCoords;

layout (location = 1) in vec3 pos;

layout (location = 0 ) out vec4 position ;
layout (location = 1 ) out vec4 normal ;
layout (location = 2 ) out vec4 diffuse ;
layout (location = 3 ) out vec4 material;


layout(set = 1, binding = 0) uniform sampler2D  color;

layout(set = 1, binding = 1) uniform MaterialBlock
{
    vec3 albedo;
    vec3 emissive;
    vec3 metallicRoughness;
   
   
} ubo;



void main() {
 	vec3 col = texture(color,TexCoords*ubo.emissive.xy).rgb + ubo.albedo;

    
diffuse = vec4(col ,1);
 normal = vec4(0);position = vec4(0);material = vec4(0);
}