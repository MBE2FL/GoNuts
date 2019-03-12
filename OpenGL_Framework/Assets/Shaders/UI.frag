#version 420

layout(binding = 0)uniform sampler2D uTex;


in vec2 texCoord;
in vec3 normal;
in vec3 position;

out vec4 outColour;


void main()
{
	vec2 textureCoord = vec2(texCoord.x, -texCoord.y);

	vec4 textureColour = texture(uTex, textureCoord);
	
	outColour.a = textureColour.a;
	outColour.rgb = textureColour.rgb;
	outColour = vec4(1.0, 0.0, 0.0, 1.0);
}