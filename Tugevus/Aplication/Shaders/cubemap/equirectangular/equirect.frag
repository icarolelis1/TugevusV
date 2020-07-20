#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 WorldPos;

layout(location = 0) out vec4 cubeMap;
layout(location = 1) in vec2 inUV;


layout(binding = 0) uniform sampler2D equirectCubeMap;


const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


void main()
{         
     vec2 uv = SampleSphericalMap(normalize(WorldPos));
     vec3 color = texture( equirectCubeMap , uv).rgb;
    	
    cubeMap = vec4(color,1.);

  
}



