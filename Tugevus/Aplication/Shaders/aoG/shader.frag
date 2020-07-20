#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0 ) out float ao;


layout(set = 0, binding = 0) uniform sampler2D  Position;
layout(set = 0, binding = 1) uniform sampler2D Normal;
layout(set = 0, binding = 2) uniform sampler2D noise;

layout(set = 0, binding = 3) uniform uniformObject{
	vec3 kernel[64];
	
}ubo;

layout(set = 0, binding = 4, std140) uniform uniformObject2{
	
	mat4 projection;
	mat4 view;
	float radius;

}ubo2;


layout(location = 0) in vec2 TexCoords;


const vec2 noiseScale = vec2(1024/4.,1024/4.);


void main(){
	vec3 fragPos =	vec4(vec4(texture(Position,TexCoords).xyz,1.0)).rgb;
	vec3 normal = normalize( texture(Normal,TexCoords).rgb);
	vec3 randomVec = texture(noise,TexCoords).rgb;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec,normal));
	vec3 bitangent = cross(normal,tangent);
	mat3 TBN = mat3(tangent,bitangent,normal);

	float occlusion = .0;
	float bias = .025;
if(ubo2.radius != .0){
	for(int i=0 ; i< 56; i++){

		vec3 samp = TBN * ubo.kernel[i];
		samp += fragPos + samp*ubo2.radius;

		vec4 offset = vec4(samp,1.0);
		offset = (ubo2.projection * ubo2.view)*offset;
		
		offset.xyz/=offset.w;
		offset.xyz = (offset.xyz * .5) + .5;
			
		float sampDepth = texture(Position,offset.xy).z;
		float range = smoothstep(.0,1.0,ubo2.radius/abs(fragPos.z - sampDepth));	
		occlusion += (sampDepth >= samp.z + bias? 1.0 : .0)*range;

	}

	occlusion = (occlusion/56.0);
	ao =1 -  occlusion;
}

else{
	ao = 1;
}
}