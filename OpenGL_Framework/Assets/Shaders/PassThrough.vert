#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out vec2 texcoord;
out vec3 normal;
out vec3 position;

void main()
{
	texcoord = in_uv;
	texcoord.y = 1 - texcoord.y;
	
	//normal = (uModel * vec4(in_normal, 0.0f)).xyz;
	normal = mat3(uView) * mat3(uModel) * in_normal;


	vec4 viewSpace = uView * uModel * vec4(in_vert, 1.0f);

	gl_Position = uProj * viewSpace;

	position = viewSpace.xyz;
}