#version 420

uniform vec4 lightPosition;

// Colour
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// Scalars
uniform float lightSpecularExponent;
uniform float attenuationConstant;
uniform float attenuationLinear;
uniform float attenuationQuadratic;

//uniform sampler2D uTex;

layout(binding = 0) uniform sampler2D uTexWater;

in vec2 texCoord;
in vec3 normal;
in vec3 position;

out vec4 outColour;


void main()
{
	outColour.rgb = lightAmbient;

	// Account for rasterizer interpolating
	vec3 tempNormal = normalize(normal);

	vec3 lightVector = lightPosition.xyz - position;
	float dist = length(lightVector);
	vec3 lightDir = lightVector / dist;

	float NdotL = dot(normal, lightDir);

	if (NdotL > 0.0)
	{
		// The light contributes to this surface

		// Calculate attenuation (falloff)
		float attenuation = 1.0 / (attenuationConstant + (attenuationLinear * dist) + (attenuationQuadratic * dist * dist));

		// Calculate diffuse contribution
		outColour.rgb += lightDiffuse * NdotL * attenuation;

		// Blinn-Phong half vector
		// eyePosition - position -> eyePosition is vec3(0, 0, 0)
		float NdotHV = max(dot(tempNormal, normalize(lightDir + normalize(-position))), 0.0);

		// Calculate specular contribution
		outColour.rgb += lightSpecular * pow(NdotHV, lightSpecularExponent) * attenuation;
	}

	vec4 textureColour = texture(uTexWater, texCoord);
	outColour.rgb *= textureColour.rgb;
	outColour.a = textureColour.a;

}