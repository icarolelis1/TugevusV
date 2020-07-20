#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 InUv;


layout (location = 0) out vec3 Normals;
layout (location = 1) out vec2 uv;
layout (location = 2) out vec4 fragPos;
layout (location = 4) out vec4 ClipSpace;
layout (location = 5) out vec4 lightSpacePosition ; 

layout(set =0,binding = 0) uniform UniformBufferObject{
		mat4 model;
	mat4 view;
	mat4 projection;
	mat4 lightMatrix;
	float time;


}ubo;
void main() {
   Normals =  inNormal* mat3(ubo.model);
   uv = InUv;
   mat4 r = mat4(ubo.view);
   fragPos = ubo.model *vec4(inPosition,1.0);
   lightSpacePosition = ubo.lightMatrix *fragPos;
   gl_Position =   ubo.projection* ubo.view * fragPos;

 
    Normals =   normalize(mat3(ubo.model) * inNormal);
    
   
    uv = InUv;
}