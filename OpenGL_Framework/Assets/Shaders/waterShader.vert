#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform float uTime;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out vec2 texCoord;
out vec3 normal;
out vec3 position;

void main()
{
	texCoord = in_uv;
//	texCoord.y = 1 - texCoord.y;
//	texCoord *= 10;
//	texCoord.y += uTime;
//	texCoord.x = sin(uTime + texCoord.y * 4)* 0.1+ texCoord.x;
	//texCoord.y = sin(uTime);
	vec3 new_in_vert = in_vert.xyz;
	//new_in_vert.x += sin((0.5f * uTime) + (new_in_vert.z * 2.5f)) * 0.03f;
	new_in_vert.y += cos((2.0f * uTime) + (new_in_vert.z * 5.0f)) * 0.01f;
	new_in_vert.z += sin((2.0f * uTime) + (new_in_vert.x * 2.5f)) * 0.01f;
	
	//normal = (uModel * vec4(in_normal, 0.0f)).xyz;
	normal = mat3(uView) * mat3(uModel) * in_normal;


	vec4 viewSpace = uView * uModel * vec4(new_in_vert, 1.0f);

	gl_Position = uProj * viewSpace;

	position = viewSpace.xyz;


//	position = (uView * uModel * vec4(in_vert, 1.0f)).xyz;
//	position.y += sin(uTime + texCoord.y * 4.0f) * 5.0f;
//
//	gl_Position = uProj * vec4(position, 1.0f);
}