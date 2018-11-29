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


uniform vec4 spotLightPosition;
uniform vec3 spotLightDirection;

// Colour
uniform vec3 spotLightAmbient;
uniform vec3 spotLightDiffuse;
uniform vec3 spotLightSpecular;

// Scalars
uniform float spotLightSpecularExponent;
uniform float spotLightattenuationConstant;
uniform float spotLightattenuationLinear;
uniform float spotLightattenuationQuadratic;

uniform sampler2D uTex;

in vec2 texCoord;
in vec3 normal;
in vec3 position;

out vec4 outColour;


void main()
{
	//outColour = vec4(normal, 1.0f);

	outColour.rgb = lightAmbient;

	// Account for rasterizer interpolating
	vec3 tempNormal = normalize(normal);

	vec3 lightVector = lightPosition.xyz - position;
	float dist = length(lightVector);
	vec3 lightDir = lightVector / dist;

	float NdotL = dot(tempNormal, lightDir);
	


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

	vec3 spotLightVector = spotLightPosition.xyz - position;
	float spotDist = length(spotLightVector);
	float angleBetweenLightAndVert = (dot(spotLightVector, vec3(0.0,0.0,-1.0)) / (spotDist * spotDist));
	angleBetweenLightAndVert = acos(angleBetweenLightAndVert);
	vec3 spotLightDir = spotLightVector / spotDist;

	float NdotSL = dot(tempNormal, spotLightDir);

	if (NdotSL > 0.0 && angleBetweenLightAndVert < 1.552)
	{
		// The light contributes to this surface
	
		// Calculate attenuation (falloff)
		float spotLightattenuation = 1.0 / (spotLightattenuationConstant + (spotLightattenuationLinear
		* spotDist) + (spotLightattenuationQuadratic * spotDist));
	
		// Calculate diffuse contribution
		outColour.rgb += spotLightDiffuse * NdotSL * spotLightattenuation;
	
		// Blinn-Phong half vector
		// eyePosition - position -> eyePosition is vec3(0, 0, 0)
		float NdotHV = max(dot(tempNormal, normalize(spotLightDir + normalize(-position))), 0.0);
	
		// Calculate specular contribution
		outColour.rgb += spotLightSpecular * pow(NdotHV, spotLightSpecularExponent) * spotLightattenuation;
	}




	vec4 textureColour = texture(uTex, texCoord);
	outColour.rgb *= textureColour.rgb;
	outColour.a = textureColour.a;

}