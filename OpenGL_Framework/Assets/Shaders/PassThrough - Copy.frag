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
uniform sampler2D uTexLookUp;

in vec2 texCoord;
in vec3 normal;
in vec3 position;

out vec4 outColour;

const float levels = 3;

void main()
{
	//outColour = vec4(normal, 1.0f);
	vec2 textureCoord = vec2(texCoord.x, -texCoord.y);

	vec4 textureColour = texture(uTex, textureCoord);
	



	vec3 P = position;
	vec3 N = normalize(normal);
	
	float dist = length(lightPosition.xyz - P);

	float attenuation = 1.0 / (attenuationConstant + (attenuationLinear * dist) + (attenuationQuadratic * dist * dist));

	

	vec3 L = normalize(lightPosition.xyz - P);

	float diffuseLight = max(dot(N, L), 0);
	vec4 textureLookUp = texture(uTexLookUp, vec2(-diffuseLight, 0.5));

	vec3 ambient = lightAmbient;

	vec3 diffuse = lightDiffuse + (textureLookUp.r * dot(N, L)) * attenuation;

	//compute specular term
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float specularLight = pow(max(dot(N, H), 0), lightSpecularExponent);

	if (diffuseLight <= 0) specularLight = 0;
	vec3 specular = lightSpecular * specularLight * attenuation;

	outColour.rgb = ambient + diffuse + specular;
	outColour.a = textureColour.a;
	outColour.rgb *= vec3(0.5,0.5,0.5);
}