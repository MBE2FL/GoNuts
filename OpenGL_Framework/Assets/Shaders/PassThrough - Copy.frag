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

layout(binding = 0)uniform sampler2D uTexDepth;
layout(binding = 1)uniform sampler2D uTex;
layout(binding = 2)uniform sampler2D uTexNormal;

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) in vData o;


out vec4 outColour;

const float levels = 3;

void main()
{
	vec2 textureCoord = vec2(o.texcoord.x, -o.texcoord.y);

	vec4 textureColour = texture(uTex, textureCoord);
	vec3 N;
	vec4 NormalTex = texture(uTexNormal, textureCoord);
	//if (NormalTex.rgb != vec3(0,0,0)) N = NormalTex.rgb; 
	N = NormalTex.rgb;

	vec3 P = o.pos;
	vec3 L = normalize(lightPosition.xyz - P);

	float diffuseLight = max(dot(N, L), 0.05);
	diffuseLight * 0.5 + 0.5;
	diffuseLight = clamp(diffuseLight, 0.05, 0.95);
	//vec4 textureLookUp = texture(uTexLookUp, vec2(diffuseLight, 0));	

	vec3 ambient = lightAmbient;

	//vec3 diffuse = lightDiffuse * textureLookUp.r;

	//compute specular term
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);

	float specularLight = pow(max(dot(N, H), 0), lightSpecularExponent);
	specularLight = clamp(specularLight, 0.05, 0.95);
	//textureLookUp = texture(uTexLookUp, vec2(specularLight, 0));

	//vec3 specular = lightSpecular * textureLookUp.r;

//	outColour.rgb = ambient + diffuse + specular;
	//outColour.rgb = ambient + diffuse;
	outColour.a = 1;
	outColour.rgb *= textureColour.rgb;
	outColour.rgb = vec3(1,1,0);
}