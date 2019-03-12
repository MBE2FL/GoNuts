/*
===========================================================================

SpeedStorm Shader Source Code
Copyright (C) 2015-2017 Stephen Thompson

===========================================================================
*/

#version 420 //Version of OpenGL we're using. - 4.2

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



layout(binding = 0) uniform sampler2D uSceneTex; 
layout(binding = 1) uniform sampler2D uSceneNormal; 
layout(binding = 2) uniform sampler2D uSceneDepth; 
//layout(binding = 30) uniform sampler3D uLUTTex;
layout(std140, binding = 4) uniform Resolution
{
	uniform vec2 uResolution;
	uniform vec2 uPixelSize;
};


uniform bool outline;
uniform mat4 uProjInverse;
uniform vec4 POS;

//uniform float uAmount = 1.0f;
in vec2 texcoord;
out vec4 outColor;

//mat3 sx = mat3( 
//    1.0, 2.0, 1.0, 
//    0.0, 0.0, 0.0, 
//   -1.0, -2.0, -1.0 
//);
//mat3 sy = mat3( 
//    1.0, 0.0, -1.0, 
//    2.0, 0.0, -2.0, 
//    1.0, 0.0, -1.0 
//);
mat3 sx;
mat3 sy;



void main()
{
	
	sx = mat3( 
	    0.0, 2.0, 0.0, 
	    0.0, 0.0, 0.0, 
	    0.0, -2.0, 0.0 
	);
	sy = mat3( 
	    0.0, 0.0, 0.0, 
	    2.0, 0.0, -2.0, 
	    0.0, 0.0, 0.0 
	);

	//vec4 source = texture(uSceneTex, texcoord);
	vec3 diffuse = texture(uSceneTex, texcoord).rgb;
	if (outline)
	{
		mat3 I;
		for (int i=0; i<3; i++) {
			for (int j=0; j<3; j++) {
				vec3 sampl  = texelFetch(uSceneTex, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
				I[i][j] = length(sampl); 
			}
		}
		

		float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
		float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);
		
		float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
		outColor = vec4(diffuse - vec3(g), 1.0);
	}
	else
	{
		outColor.rgb = diffuse;
	}


	//outColor.rgb = vec3(1,0,0);
	outColor.a = 1.0;

	// Calculate texture coordinate position on screen
	vec2 texOffset = gl_FragCoord.xy * uPixelSize;
	
	float depth =  texture(uSceneDepth, texOffset).r;
	if(depth == 1) discard;
	vec4 position = uProjInverse * (vec4(texOffset, depth, 1.0) * 2 - 1);
	position /= position.w;
	
	vec4 albedoColor = texture(uSceneTex, texOffset);
	//outColor.rgb = vec3(0); 
	
	vec3 normal = normalize(texture(uSceneNormal, texOffset).rgb * 2 - 1);

	vec3 lightDir = normalize(POS.xyz - position.xyz);
	float NdotL = dot(normal, lightDir);		
	// Clamp NdotL so that there are negative values, otherwise 
	// the opposite side of an object would receive negative lighting!
	NdotL = max(NdotL, 0.0);
	outColor.rgb += vec3(0.6) * NdotL;
	//outColor.rgb = normal;

}