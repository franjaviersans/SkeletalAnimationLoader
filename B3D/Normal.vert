#version 450



const int MAX_BONES = 100;

uniform mat4 modelView;
uniform mat4 Projection;
uniform mat4 gBones[MAX_BONES];

layout(location = 0) in vec4 glVertex;
layout(location = 1) in vec3 glNormal;
layout(location = 2) in vec2 glTexCoord;
layout(location = 3) in ivec4 glBoneIDs;
layout(location = 4) in vec4 glWeights;

out vec3 vecPos;
out vec2 Tex;

void main()
{

	mat4 BoneTransform = gBones[glBoneIDs[0]] * glWeights[0];
		BoneTransform += gBones[glBoneIDs[1]] * glWeights[1];
		BoneTransform += gBones[glBoneIDs[2]] * glWeights[2];
		BoneTransform += gBones[glBoneIDs[3]] * glWeights[3];

	//mat4 BoneTransform = mat4(1.0f);

	vecPos = (Projection * modelView * BoneTransform * glVertex).xyz;
	Tex = glTexCoord;
	gl_Position = (Projection * modelView * BoneTransform  * glVertex);
}

