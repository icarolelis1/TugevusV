#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 InVelocity;




layout (location = 0) out vec3 Color;
layout (location = 1) out vec2 outUV;


layout(set =0,binding = 0) uniform UniformBufferObject{
	
	mat4 projection;
	mat4 view;

}ubo;



void main() {
outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    mat4 projView = ubo.projection * ubo.view;
    gl_Position = projView* vec4(inPosition,1.0);

    Color = vec3(inPosition.y);;
    gl_PointSize = 60;
}