#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout(location = 0) out vec3 WorldPos;
layout(location = 1) out float Time;


layout(set =0,binding = 0) uniform UniformBufferObject {
   mat4 proj;
   mat4 view;
   float time;
} ubo;


void main()
{		mat4
rotView= mat4(mat3(ubo.view));
		vec4 clipPos = ubo.proj*rotView * vec4(aPos,1.);
		
		WorldPos = aPos;
		WorldPos.y*=-1;
		Time = ubo.time;
		gl_Position = clipPos.xyww;
}