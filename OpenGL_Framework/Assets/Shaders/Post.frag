/*
===========================================================================

SpeedStorm Shader Source Code
Copyright (C) 2015-2017 Stephen Thompson

===========================================================================
*/

#version 420 //Version of OpenGL we're using. - 4.2

uniform sampler2D uSceneTex; 
//layout(binding = 30) uniform sampler3D uLUTTex;
uniform bool outline;

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

mat3 sx = mat3( 
    0.0, 2.0, 0.0, 
    0.0, 0.0, 0.0, 
    0.0, -2.0, 0.0 
);
mat3 sy = mat3( 
    0.0, 0.0, 0.0, 
    2.0, 0.0, -2.0, 
    0.0, 0.0, 0.0 
);

void main()
{
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


	//outColor.rgb = source.rgb;
	outColor.a = 1.0;

}