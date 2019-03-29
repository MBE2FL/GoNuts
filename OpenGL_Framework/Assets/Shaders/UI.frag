#version 420

layout(binding = 0)uniform sampler2D uTex;

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) in vData o;

out vec4 outColour;

uniform float uAlpha = 1.0;

void main()
{
	vec2 textureCoord = o.texcoord;

	vec4 textureColour = texture(uTex, textureCoord);

	if (textureColour.a == 0)
		discard;
	
	outColour.a = uAlpha;
	outColour.rgb = textureColour.rgb;
}