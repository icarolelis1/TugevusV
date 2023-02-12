#version 450

struct light{

    vec3 lightPos;
    vec3 lightColor;
    float type;
    float factor;
};
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec4 WorldPos;
layout(location = 0) in vec3 Normals;
layout(location = 4) in vec4 ClipSpace;
layout (location = 5) in vec4 lightSpacePosition ; 




layout(set = 2, binding = 0, std140) uniform UniformBufferObject
{
      mat4 lightMatrix;
	mat4 invVIew;
	mat4 invProj;

    vec3 eyePos;
   light lights[5];
   int numberOfLights;
} lightUbo;


layout(set = 1, binding = 0) uniform sampler2D normalMap ;
layout(set = 1, binding = 1) uniform sampler2D dudvMap;
layout(set = 1, binding = 2) uniform samplerCube skyCubeMap;
layout(set = 1, binding = 3) uniform samplerCube irradianceMap;
layout(set = 1, binding = 5) uniform sampler2D refractionMap;
layout(set = 1, binding = 6) uniform sampler2D depthMap;



layout (set = 1,binding = 4)  uniform Parameters{
	float strengh;
	vec3 texCoordsScale;
	
	float time;
	float reflectivity;

}parameterUbo ;


layout (location = 0 ) out vec4 position ;
layout (location = 1 ) out vec4 Normal;
layout (location = 2 ) out vec4 diffuse ;
layout (location = 3 ) out vec4 material;

float reflectivity = parameterUbo.reflectivity;
float strenght = parameterUbo.strengh;
float time = parameterUbo.time;



    float shadowCalculation(vec4 fragPosLightSpace, float bias)
    {
    return .0;
        vec3 projCoords = fragPosLightSpace.xyz / vec3(fragPosLightSpace.w);

        projCoords = (projCoords * 0.5) + vec3(0.5);

        float closestDepth = texture(depthMap, projCoords.xy).x * .5 + .5 ;

        float currentDepth = projCoords.z;

        float shadow = 0.0;
        vec2 texelSize = 1/textureSize(depthMap,0);
        /*
        for(int x = -4;x<=4;x++){
            for(int y = -4;y<=4;y++){
                float pcdepth = texture(depthMap,projCoords.xy + vec2(x,y) * texelSize).r * .5 + .5;
                if(currentDepth > pcdepth)shadow+=.2;
                else{
                    shadow+=1.;
                }
            
            }
        
        }

        return shadow/16.;
       */
       // if(currentDepth > closestDepth  )return 0.2;
       // return 1.;
    }

void main()
{



vec2 texC = vec2(TexCoords.x ,TexCoords.y);
texC *= parameterUbo.texCoordsScale.xy;
vec2 distortedTexCoords = texture(dudvMap,vec2(texC.x + time ,texC.y)).rg*.1;
distortedTexCoords = TexCoords + vec2(distortedTexCoords.x,distortedTexCoords.y+time);
vec2 totalDistortion = strenght * (texture(dudvMap,distortedTexCoords).rg*2. -1.);


vec3 viewDir = normalize(lightUbo.eyePos - WorldPos.xyz );

vec3 L = normalize(lightUbo.lights[0].lightPos - WorldPos.xyz);
float bias = max(0.05 * (1.0 - dot(vec3(0,-1,0), L)), 0.05);  

vec3 ndc = lightSpacePosition.xyz * .5 + vec3(.5);
ndc = ndc/lightSpacePosition.w;

float shadow = texture(depthMap,ndc.xy).r;


float refractiveIndex = dot(viewDir,vec3(0,1,0));
refractiveIndex = pow(refractiveIndex ,.5);


vec3 normal = texture(normalMap,(totalDistortion )).rgb;
normal = vec3((normal.r *2.0) -1.,normal.b, (normal.g *2.) -1.);

normal = normalize(normal);
vec3 t = reflect(viewDir,normal);
vec3 ref = texture(skyCubeMap,t).rgb;


vec3 col = vec3(0);


for(int i=0;i< lightUbo.numberOfLights ;i++){
 

    vec3 lightDir = normalize(lightUbo.lights[i].lightPos - WorldPos .xyz);

    vec3 reflectDir = normalize(reflect(lightDir,normal));
 
   
    float   spec = pow(max(dot( reflectDir ,viewDir ), 0.0), 4);


    float attenuation =1.;

if(lightUbo.lights[i].type == 1.) {
	
       float distance = distance(lightUbo.lights[i].lightPos,WorldPos.xyz);
       float fatt =  (lightUbo.lights[i].factor*distance*distance);
	    attenuation = 1./fatt;
 }



    vec3 specularLight = lightUbo.lights[i].lightColor * spec *reflectivity *attenuation;
 

      

	col+= specularLight;
        

}


vec3 refr = texture(refractionMap,TexCoords*4.).rgb*.4;
col +=   mix(ref,refr, refractiveIndex );
col*=shadow;
diffuse = vec4(col ,1);
Normal =vec4(normal,1.0) ;material = vec4(parameterUbo.reflectivity,0,0,2) ;position = WorldPos;
 
}    