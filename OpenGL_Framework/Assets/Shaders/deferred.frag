#version 420

layout(std140, binding = 4) uniform Resolution
{
	uniform vec2 uResolution;
	uniform vec2 uPixelSize;
};

layout(binding = 0) uniform sampler2D uSceneTex; 
layout(binding = 1) uniform sampler2D uSceneNormal; 
layout(binding = 2) uniform sampler2D uSceneDepth; 
layout(binding = 5) uniform sampler2D uSceneToon; 


uniform mat4 uProjInverse;

uniform int numLights;
uniform vec4 uLightColor;
uniform vec4 uLightPosition;
uniform vec4 uLightAttenuation;

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) in vData o;
out vec4 outColor;

void main()
{
	vec2 texOffset = gl_FragCoord.xy * uPixelSize;

	float depth =  texture(uSceneDepth, texOffset).r;
	if(depth == 1) discard;
	vec4 position = uProjInverse * (vec4(texOffset, depth, 1.0) * 2 - 1);
	position.xyz /= position.w;

	vec4 albedoColor = texture(uSceneTex, texOffset);
	outColor.rgb = albedoColor.rgb; 

	vec3 normal = normalize(texture(uSceneNormal, texOffset).rgb * 2 - 1);

	
	vec3 lightVec = uLightPosition.xyz - position.xyz;
	
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	float NdotL = dot(normal, lightDir);

	// Normalized vector pointing towards the camera
	vec3 eye = normalize(-position.xyz);
	
	// Calculate attenuation (falloff)
	float attenuation = 1.0 /(uLightAttenuation.x + dist * uLightAttenuation.y + dist * dist * uLightAttenuation.z);

	NdotL = NdotL * 0.5 + 0.5;
	// Calculate the diffuse contribution, substituting the NdotL into a color ramp
	outColor.rgb += albedoColor.rgb * uLightColor.xyz * texture(uSceneToon, vec2(NdotL, 0.5)).rgb * attenuation;
	//outColor .rgb = fract(lightVec);
	

	//outColor.rgb = uLightColor.xyz; 
	outColor.a = 1.0;
	//outColor.rgb += texture(uTexEmissive, texOffset).rgb;
}
