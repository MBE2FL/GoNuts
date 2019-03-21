#version 420

const int MAX_JOINTS = 70; // NEW
const int MAX_WEIGHTS = 4; // NEW

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 jointTransforms[MAX_JOINTS]; // NEW

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec4 in_jointIndices; // NEW
layout(location = 4) in vec4 in_weights; // NEW

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
	o.texcoord.y = 1.0 - o.texcoord.y;


	vec4 totalLocalPos = vec4(0.0, 0.0, 0.0, 0.0);
	totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	// Only allow MAX_WEIGHTS number of joints to influence this vertex.
	for (unsigned int i = 0; i < MAX_WEIGHTS; ++i)
	{
		mat4 jointTransform = jointTransforms[int(in_jointIndices[i])];

		// Calculate position and normal based on the positions and weights of each joint
		// affecting this vertex.
		vec4 posePos = jointTransform * vec4(in_vert, 1.0);
		totalLocalPos += posePos * in_weights[i];

		vec4 worldNormal = jointTransform * vec4(in_normal, 0.0);
		totalNormal += worldNormal * in_weights[i];
	}

	o.norm = mat3(uView) * mat3(uModel) * totalNormal.xyz;


	vec4 viewSpace = uView * uModel * totalLocalPos;

	gl_Position = uProj * viewSpace;

	o.pos = viewSpace.xyz;
}