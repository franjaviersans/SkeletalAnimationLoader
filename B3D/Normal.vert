#version 450



const int MAX_BONES = 100;

uniform mat4 modelView;
uniform mat4 Projection;
uniform mat4 gBones[MAX_BONES];

layout(location = 0) in vec4 glVertex;
layout(location = 1) in vec2 glTexCoord;
layout(location = 2) in vec3 glNormal;
layout(location = 3) in ivec4 glBoneIDs;
layout(location = 4) in vec4 glWeights;

out vec3 vecPos;
out vec2 Tex;

void main()
{

	/*mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
		BoneTransform += gBones[BoneIDs[1]] * Weights[1];
		BoneTransform += gBones[BoneIDs[2]] * Weights[2];
		BoneTransform += gBones[BoneIDs[3]] * Weights[3];*/

	mat4 BoneTransform = mat4(1.0f);

	vecPos = (Projection * modelView * BoneTransform * glVertex).xyz;
	Tex = glTexCoord;
	gl_Position = (Projection * modelView * BoneTransform  * glVertex);
}

