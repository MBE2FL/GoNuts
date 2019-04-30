#version 420

//layout(std140, binding = 11) uniform Camera
//{
//	uniform mat4 uView;
//	uniform mat4 uProj;
//};

uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 12) in mat4 in_model;

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) out vData o;

void main()
{
	o.texcoord = in_uv;
	o.texcoord.y = 1 - o.texcoord.y;

	
	//normal = (uModel * vec4(in_normal, 0.0f)).xyz;
	o.norm = mat3(uView) * mat3(in_model) * in_normal;


	vec4 viewSpace = uView * in_model * vec4(in_vert, 1.0f);

	gl_Position = uProj * viewSpace;

	o.pos = viewSpace.xyz;
}