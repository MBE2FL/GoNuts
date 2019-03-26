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


void main()
{
	vec2 textureCoord = o.texcoord;

	vec4 textureColour = texture(uTex, textureCoord);
	
	outColour.a = textureColour.a;
	outColour.rgb = textureColour.rgb;
	outColour.rgb = vec3(1,0,0);
}