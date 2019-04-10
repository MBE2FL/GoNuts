#version 420 //Version of OpenGL we're using. - 4.2

layout(binding = 0) uniform sampler2D uSceneTex; 
layout(binding = 30) uniform sampler3D uLUTTex;

uniform bool lut = true;
uniform bool screenShake = false;
uniform bool Flip = false;

uniform float totalGameTime;
uniform float uAmount = 1.0f;
in vec2 texcoord;
out vec4 outColor;

void main()
{
	vec2 texOffest = texcoord;
	if (screenShake)
	{
		texOffest.y = texOffest.y + sin(totalGameTime * 50) * 0.01;
	}
	if (Flip)
	{
		texOffest.y = 1 - texOffest.y;
	}
	vec4 source = texture(uSceneTex, texOffest);
	
	//float luminance = 0.2989 * source.r + 0.5874 * source.g + 0.1137 * source.b;
	//outColor.rgb = mix(source.rgb, vec3(luminance), uAmount);
	//outColor.a = 1.0f;
	
	vec3 scale = vec3((64.0-1.0)/64);
	vec3 offset = vec3 (1.0/(2.0*64));

	vec3 applyLUT = texture(uLUTTex, scale * source.rgb + offset).rgb;

	//if (lut)
	//{
	//	outColor.rgb = mix(source.rgb, applyLUT, uAmount);
	//}
	//else
	//{
	//	outColor.rgb = source.rgb;
	//}
	outColor.rgb = mix(source.rgb, applyLUT, lut);
	outColor.a = 1.0;

}