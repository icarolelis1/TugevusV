#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 InUv;




layout (location = 0) out vec2 uv;
layout(location = 1) out vec3 Pos;

layout(set =0,binding = 0) uniform UniformBufferObject{
		mat4 model;
	mat4 view;
	mat4 projection;
	mat4 lightMatrix;
	float time;

}ubo;



void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model*vec4(inPosition,1.0);
   // Normals = mat3(transpose(inverse(ubo.model))) * inNormal;  
  
	uv = InUv;
}