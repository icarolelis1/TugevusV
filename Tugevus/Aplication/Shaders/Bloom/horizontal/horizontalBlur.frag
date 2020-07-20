#version 450

layout(set = 0,binding = 0) uniform sampler2D mainImage;
layout(set = 0,binding = 1) uniform sampler2D verticalBlur;



layout (location = 0 ) in vec2 texCoord ;
layout (location = 0 ) out vec4 outColor ;

layout(set =0,binding = 2) uniform UniformBufferObject{

		
			float saturation;
		float contrast;
		float brightness;
		float exposure;

 }ubo;


mat4 brightnessMatrix( float brightness )
{
    return mat4( 1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 brightness, brightness, brightness, 1 );
}

mat4 contrastMatrix( float contrast )
{
	float t = ( 1.0 - contrast ) / 2.0;
    
    return mat4( contrast, 0, 0, 0,
                 0, contrast, 0, 0,
                 0, 0, contrast, 0,
                 t, t, t, 1 );

}

mat4 saturationMatrix( float saturation )
{
    vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 );
    
    float oneMinusSat = 1.0 - saturation;
    
    vec3 red = vec3( luminance.x * oneMinusSat );
    red+= vec3( saturation, 0, 0 );
    
    vec3 green = vec3( luminance.y * oneMinusSat );
    green += vec3( 0, saturation, 0 );
    
    vec3 blue = vec3( luminance.z * oneMinusSat );
    blue += vec3( 0, 0, saturation );
    
    return mat4( red,     0,green,   0, 
    blue,    0,
                 0, 0, 0, 1 );
}

const float brightness = -.01;
const float contrast = 1.7;
const float saturation = 1.2;



vec3 gaussianBlurHorizontal(){
 float weight[5] = {0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};

	vec3 result = texture(mainImage,texCoord).rgb * weight[0];

     vec2 tex_offset = 1.0 / textureSize(verticalBlur, 0); // gets size of single texel
	 tex_offset*=4;

	for(int i=1;i<5;i++){

            result += texture(mainImage, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		    result += texture(mainImage, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];		
		}
	

	return result;
}



void main() {

	vec3 c =gaussianBlurHorizontal();
	
	vec3 col = c+texture(verticalBlur,texCoord).rgb;
    //outColor = vec4(col,1.0);
	outColor = brightnessMatrix( ubo.brightness ) *
        		contrastMatrix( ubo.contrast ) * 
        		saturationMatrix(ubo.saturation) *
        	   vec4(col,1.);



}