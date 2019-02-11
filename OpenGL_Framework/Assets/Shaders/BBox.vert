#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec4 in_colour;

out vec4 colour;
out vec3 position;

void main()
{
	colour = in_colour;
	//colour = vec4(1.0f);

	vec4 viewSpace = uView * uModel * vec4(in_vert, 1.0f);

	gl_Position = uProj * viewSpace;

	position = viewSpace.xyz;
}