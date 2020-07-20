#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout(location = 0) out vec3 WorldPos;
layout(location = 1) out vec2 outUV;

layout( push_constant ) uniform pushBlock{
  mat4 view;
  mat4 proj;
	
} PushConstant;

void main()
{		
	 outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
		WorldPos = aPos;
		//mat4 rotView = mat4(mat3(PushConstant.view));
		//gl_Position = PushConstant.proj*rotView*	vec4(aPos,1.);
		gl_Position = PushConstant.proj*PushConstant.view*vec4(WorldPos,1.0);
}