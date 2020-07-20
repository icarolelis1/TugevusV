#version 450

layout(set = 1, binding = 0) uniform sampler2D albedoMap;
layout(set = 1, binding = 1) uniform sampler2D normalMap;
layout(set = 1, binding = 2) uniform sampler2D metallicMap;
layout(set = 1, binding = 3) uniform sampler2D roughnessMap;
layout(set = 1, binding = 4) uniform sampler2D emissionMap;
layout(set = 1, binding = 6) uniform sampler2D aoMap;
layout(set = 1, binding = 7) uniform sampler2D displacementMap;

layout(location = 0) in vec3 Normals;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec3 WorldPos;


layout (location = 0 ) out vec4 position ;
layout (location = 1 ) out vec4 normal ;
layout (location = 2 ) out vec4 diffuse ;
layout (location = 3 ) out vec4 material;

layout(set = 1, binding = 5) uniform MaterialBlock
{
		vec3	albedo;
		vec3	textureScaleFactor;
		vec3   metallicRoughness;
		bool hasNormalMap;
   
   
} Material;

mat3 getTBN(){

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normals);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return TBN;
}


vec3 getNormalFromMap(vec2 scaleFactor)
{
    vec3 tangentNormal = texture(normalMap, TexCoords * scaleFactor).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normals);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

/*
vec2 parallaxCoords(vec2 texcoords,vec3 viewDir,float heightScale,float bias,vec2 scaleFactor){
    
    float height = texture(displacementMap,texcoords * scaleFactor).r;
    vec2 p = viewDir.xy * height * (heightScale +bias);
    return p/viewDir.z;

}*/

void main()
{


    vec2 scaleFactor = Material.textureScaleFactor.xy;
        

    if(Material.hasNormalMap)
    normal = (vec4(getNormalFromMap(scaleFactor) ,1.0));
else{
     normal = vec4(Normals,1.0);
}
    vec3 albedo =   pow(texture( albedoMap , TexCoords * scaleFactor  ).xyz  ,vec3(2.2)) + Material.albedo ;
    float metallic =   texture(metallicMap,TexCoords).r  * Material.metallicRoughness.x;
    float roughness =    texture(roughnessMap, TexCoords * scaleFactor).x  *  Material.metallicRoughness.g;
    float ao =   texture(aoMap, TexCoords * scaleFactor  ).r ;

position = vec4(WorldPos  ,1.0);
diffuse = vec4(albedo,1.0);
material = vec4(metallic, roughness, ao,1.0);


}    