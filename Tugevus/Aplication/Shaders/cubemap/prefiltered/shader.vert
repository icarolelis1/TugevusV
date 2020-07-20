#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 worldPos;

layout(push_constant) uniform PushConsts {	
		mat4 view;
		mat4 projection;
		float roughness;
} pushConsts;


void main()
{		worldPos = aPos;
		gl_Position = pushConsts.projection * pushConsts.view * vec4(aPos,1.);
}