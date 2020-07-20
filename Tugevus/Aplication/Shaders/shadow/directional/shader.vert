#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 InUv;



layout(set =0 , binding = 0) uniform UniformBufferObject2{

	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 lightMatrix;
	float time;

}ubo2;


void main() {


  vec4 fragPos  = ubo2.model * vec4(inPosition,1.0) ;
    gl_Position = ubo2.lightMatrix* fragPos;
  
}