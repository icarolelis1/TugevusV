#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 WorldPos;
layout(location = 1) in float Time;


layout (location = 0 ) out vec4 position ;
layout (location = 1 ) out vec4 normal ;
layout (location = 2 ) out vec4 diffuse ;
layout (location = 3 ) out vec4 material;

layout(binding = 1) uniform samplerCube cubeMap;

const int MAX_DIST = 1000;
const float EPSI = 0.009;




void main()
{         
	//textureLod(environmentMap, WorldPos, 1.2).rgb; 
    vec3 color= textureLod(cubeMap, WorldPos,.0).rgb;
  	 
 
   

    diffuse= vec4(color ,1.);

    position = vec4(0);normal = vec4(0);material = vec4(0);
 
}



