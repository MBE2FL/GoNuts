#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform float T;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_vert_2;

out vec2 texCoord;
out vec3 normal;
out vec3 position;

void main()
{
	texCoord = in_uv;
	//normal = (uModel * vec4(in_normal, 0.0f)).xyz;
	normal = mat3(uView) * mat3(uModel) * in_normal;

	vec3 P = mix(in_vert, in_vert_2, T);

	vec4 viewSpace = uView * uModel * vec4(P, 1.0f);

	gl_Position = uProj * viewSpace;

	position = viewSpace.xyz;
}