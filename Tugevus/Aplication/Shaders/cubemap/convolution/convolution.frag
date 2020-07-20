#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 worldPos;
layout (location = 0) out vec4 outColor;

layout (set = 0,binding = 0) uniform samplerCube samplerEnv;

#define PI 3.1415926535897932384626433832795

void main()
{         
	
	vec3 N = normalize(worldPos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, N));
	up = cross(N, right);

    const float TWO_PI = PI * 2.0;
    const float HALF_PI = PI * 0.5;


uint sampleCount = 0u;
const float samplerDelta = .025;
vec3 color = vec3(0);

for (float phi = 0.0; phi < TWO_PI; phi += samplerDelta ) {
		for (float theta = 0.0; theta < HALF_PI; theta += samplerDelta ) {
			vec3 tempVec = cos(phi) * right + sin(phi) * up;
			vec3 sampleVector = cos(theta) * N + sin(theta) * tempVec;
			color += texture(samplerEnv, sampleVector).rgb* cos(theta) * sin(theta);
			sampleCount++;
		}
	}
 
	vec4 col = vec4(PI * color *(1/ float(sampleCount)), 1.0);
        
	outColor = col;

  	


}



