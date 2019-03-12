#version 440

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

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
	
	o.norm = mat3(uView) * mat3(uModel) * in_normal;

	o.pos = (uView * uModel * vec4(in_vert, 1.0f)).xyz;

	gl_Position = uProj * vec4(o.pos, 1.0f);
}