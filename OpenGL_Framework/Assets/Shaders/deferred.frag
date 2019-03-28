#version 420

//layout(std140, binding = 3) uniform Light
//{
//	uniform vec3 uLightColor;
//	uniform vec3 uLightPosition;
//	uniform vec3 uLightDirection;
//	uniform vec4 uLightAttenuation; 
//};
//
//#define uLightConstantAttenuation uLightAttenuation.x
//#define uLightLinearAttenuation uLightAttenuation.y
//#define uLightQuadAttenuation uLightAttenuation.z
//#define uLightRadius uLightAttenuation.w
//
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
uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform vec3 uLightDirection;
uniform vec4 uLightAttenuation;
#define uLightConstantAttenuation uLightAttenuation.x
#define uLightLinearAttenuation uLightAttenuation.y
#define uLightQuadAttenuation uLightAttenuation.z
#define uLightRadius uLightAttenuation.w


//uniform float uAmount = 1.0f;
in vec2 texcoord;
out vec4 outColor;

void main()
{
	vec2 texOffset = gl_FragCoord.xy * uPixelSize;

	float depth =  texture(uSceneDepth, texOffset).r;
	if(depth == 1) discard;
	vec4 position = uProjInverse * (vec4(texOffset, depth, 1.0) * 2 - 1);
	position.xyz /= position.w;

	vec4 albedoColor = texture(uSceneTex, texOffset);
	outColor.rgb = vec3(0); 

	vec3 normal = normalize(texture(uSceneNormal, texOffset).rgb * 2 - 1);

	vec3 lightVec = uLightPosition - position.xyz;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	float NdotL = dot(normal, lightDir);

	// If the normal is facing the light
	
	// Normalized vector pointing towards the camera
	vec3 eye = normalize(-position.xyz);
	
	// Calculate attenuation (falloff)
	// Add a small number to avoid divide by zero.
	float attenuation = 1.0 / (uLightConstantAttenuation + dist * uLightLinearAttenuation + dist * dist * uLightQuadAttenuation);

	NdotL = NdotL * 0.5 + 0.5;
	// Calculate the diffuse contribution, substituting the NdotL into a color ramp
	outColor.rgb += albedoColor.rgb * uLightColor * texture(uSceneToon, vec2(NdotL, 0.5)).rgb * attenuation;
	//outColor.rgb = lightVec;
	//outColor.rgb += texture(uTexEmissive, texOffset).rgb;
}
