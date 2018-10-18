#version 420

out vec4 outColour;

in vec3 normal;

void main()
{
	outColour = vec4(normal, 1.0f);
}