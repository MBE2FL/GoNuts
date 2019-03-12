#version 420

layout(binding = 0) uniform sampler2D uTexAlbedo;

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) in vData o;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNormal;

void main()
{
	vec2 texOffset = o.texcoord;

	vec4 albedoColor = texture(uTexAlbedo, texOffset);
	if(albedoColor.a < 0.5)	discard;
	
	outColor = albedoColor.rgb;

	// Fix length after rasterizer interpolates
	outNormal = normalize(o.norm) * 0.5 + 0.5;
}