#version 450

layout(set = 0,binding = 0) uniform sampler2D mainImage;
layout(set = 0,binding = 1) uniform sampler2D verticalBlur;

layout (location = 0 ) in vec2 texCoord ;
layout (location = 0 ) out vec4 outColor ;


vec3 gaussianBlurHorizontal(){
 float weight[5] = {0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};

	vec3 result = texture(mainImage,texCoord).rgb * weight[0];

     vec2 tex_offset = 1.0 / textureSize(verticalBlur, 0); // gets size of single texel


	for(int i=0;i<5;i++){

            result += texture(mainImage, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		    result += texture(mainImage, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];		
		}
	

	return result;
}


void main() {

	vec3 c =gaussianBlurHorizontal();
	outColor = vec4(c + texture(verticalBlur,texCoord).rgb,1.);

}