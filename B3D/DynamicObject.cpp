// $Id: $
//
// Author: Francisco Sans franjaviersans@gmail.com
//
// Complete history on bottom of file

#define FILE_REVISION "$Revision: $"


#include "DynamicObject.h"
#include <stdio.h>
#include <iostream>
using namespace std;

void print(const glm::mat4 & m){
	cout << "Begin:"
		<< "{ "		<< m[0][0] << " , " << m[0][1] << " , " << m[0][2] << " , " << m[0][3] << endl
					<< m[1][0] << " , " << m[1][1] << " , " << m[1][2] << " , " << m[1][3] << endl
					<< m[2][0] << " , " << m[2][1] << " , " << m[2][2] << " , " << m[2][3] << endl
					<< m[3][0] << " , " << m[3][1] << " , " << m[3][2] << " , " << m[3][3] << "} " << endl;
}

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 from)
{
	glm::mat4 to;


	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
	to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

	return to;
}

/**
* Default constructor
*
* @params velocity the velocity of the animation
* @params Width the width of the texture atlas
* @params Height the height of the texture atlas
* @params start the start frame of the animation
* @params end the end frame of the animation
*
*/
DynamicObject::DynamicObject(GLfloat velocity, GLuint Width, GLuint Height, GLint start, GLint end)
{
	m_fAnimationVelocity = velocity;

	m_iStart = start;
	m_iEnd = end;

	m_bb.Init();

	m_NumBones = 0;
}

/**
* Default destructor
*/
DynamicObject::~DynamicObject()
{
	glDeleteBuffers(1, &m_iVao);
}



/**
 * Load an MD2 model from file.
 *
 * Note: MD2 format stores model's data in little-endian ordering.  On
 * big-endian machines, you'll have to perform proper conversions.
 *
 * @params filename the name of the model to be loaded
 * @params TransformationMatrix a Matrix with all the transformations to be made to the model
 *
 */
void DynamicObject::Import(const char *filename, const glm::mat4 &TransformationMatrix)
{
	cout << "Loading " << filename << endl;

	// Read file via ASSIMP
	scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}


	string pFile = filename;
	// Retrieve the directory path of the filepath
	m_directory = pFile.substr(0, pFile.find_last_of('/'));


	m_userTransform = TransformationMatrix;

	m_GlobalInverseTransform = aiMatrix4x4ToGlm(scene->mRootNode->mTransformation);
	m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

	// Process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f));


	//popullate the auxiliar array of transformations
	for (GLuint i = 0; i < m_BoneInfo.size(); ++i){
		glm::mat4 m = glm::mat4(1.0f);
		m_transforms.push_back(m);
	}
	
}


Mesh DynamicObject::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4 &TransformationMatrix)
{

	if (!mesh->HasBones()) cout << "This mesh doesn't have bones" << endl;
	
	
	cout << "Testing some stuff " << mesh->mNumBones << endl;

	Mesh data;


	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Positions
		vertex.WorldCoord.x = mesh->mVertices[i].x;
		vertex.WorldCoord.y = mesh->mVertices[i].y;
		vertex.WorldCoord.z = mesh->mVertices[i].z;
		vertex.WorldCoord.w = 1.0f;

		vertex.WorldCoord = TransformationMatrix * vertex.WorldCoord; //transform vertex
		

		m_bb.AppendPoint(vertex.WorldCoord.x, vertex.WorldCoord.y, vertex.WorldCoord.z);
		
		// Normals
		vertex.NormalCoord.x = mesh->mNormals[i].x;
		vertex.NormalCoord.y = mesh->mNormals[i].y;
		vertex.NormalCoord.z = mesh->mNormals[i].z;

		vertex.NormalCoord = glm::vec3(glm::inverse(glm::transpose(TransformationMatrix)) * glm::vec4(vertex.NormalCoord, 0.0f)); //transform vertex


		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vertex.TextureCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.TextureCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.TextureCoord = glm::vec2(0.0f, 0.0f);

		data.m_vVertexInfo.push_back(vertex);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			data.m_vIndex.push_back(face.mIndices[j]);

	}




	//Process Bones
	for (GLuint j = 0; j < mesh->mNumBones; ++j){
		GLuint BoneIndex = 0;
		string BoneName(mesh->mBones[j]->mName.data);

		//get and index for the bone
		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
		}
		else{
			BoneIndex = m_BoneMapping[BoneName];
		}

		//Store the information of the bone
		m_BoneMapping[BoneName] = BoneIndex;
		m_BoneInfo[BoneIndex].m_Offset = aiMatrix4x4ToGlm(mesh->mBones[j]->mOffsetMatrix);


		//search for every where does he influences
		for (GLuint k = 0; k < mesh->mBones[j]->mNumWeights; ++k){
			GLuint vertID = mesh->mBones[j]->mWeights[k].mVertexId;
			GLfloat weight = mesh->mBones[j]->mWeights[k].mWeight;

			//store the information on the vector
			GLuint l = 0;
			for (l = 0; l < NumBones; ++l){
				if (data.m_vVertexInfo[vertID].BoneWeight[l] < 0.00001f){
					data.m_vVertexInfo[vertID].BoneWeight[l] = weight;
					data.m_vVertexInfo[vertID].BoneID[l] = BoneIndex;
					l = 100;
				} 
			}
			if (l >= NumBones && l< 100)	std::cout << "We need more bones!!!"<< std::endl;
		}

	}

	
	//create the VBO and VAO for this mesh
	data.setupMesh();

	





	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		/*aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());*/
	}

	// Return a mesh object created from the extracted mesh data
	return data;
}

void DynamicObject::ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4 &TransformationMatrix)
{

	glm::mat4 trans = aiMatrix4x4ToGlm(node->mTransformation);

	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		
		m_vMeshes.push_back(ProcessMesh(mesh, scene, TransformationMatrix * trans));
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		
		ProcessNode(node->mChildren[i], scene, TransformationMatrix * trans);
	}

}


/**
 * Calculate the current frame in animation beginning at frame
 * 'start' and ending at frame 'end', given interpolation percent.
 * interp will be reseted to 0.0 if the next frame is reached.
 *
 * @params time is the time elapsed from the previos time
 *
 */
void DynamicObject::Animate (float time)
{
	if(m_fInterpolation <= 0.0f)
		m_fInterpolation  = 0.0f;
	


	m_fInterpolation += time * m_fAnimationVelocity;

	/*if ((m_iFrame < m_iStart) || (m_iFrame > m_iEnd))
	m_iFrame = m_iStart;

	if (m_fInterpolation >= 1.0f)
	{
		/* Move to next frame 
		m_fInterpolation = 0.0f;
		(m_iFrame)++;

		if (m_iFrame >= m_iEnd)
			m_iFrame = m_iStart;
	}*/

	if (scene->mAnimations[0]->mDuration < m_fInterpolation) m_fInterpolation = 0.0f;

	float TicksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = m_fInterpolation * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, scene->mAnimations[0]->mDuration);


	//All meshes are already moved
	glm::mat4 m = glm::mat4(1.0f);

	//begin to transverse the hierarchy, updating the bone structure
	BoneHeirarchyTransform(AnimationTime, scene->mRootNode, m);

}


void DynamicObject::BoneHeirarchyTransform(float AnimationTime, const aiNode *pNode, const glm::mat4 & parentTransform)
{
	string Nodename(pNode->mName.data);
	//const aiAnimation

	const aiAnimation* pAnimation = scene->mAnimations[0];

	glm::mat4 NodeTrans = aiMatrix4x4ToGlm(pNode->mTransformation);

	//Search for the animation of the node
	const aiNodeAnim * pNodeAnim = NULL;

	for (GLuint i = 0; i < pAnimation->mNumChannels; ++i){
		if (pAnimation->mChannels[i]->mNodeName == pNode->mName){
			pNodeAnim = scene->mAnimations[0]->mChannels[i];
			break;
		}
	}

	if (pNodeAnim){
		
		NodeTrans = Interpolatedtransformation(AnimationTime, pNodeAnim);
		
	}

	
	glm::mat4 GLobaltransform = parentTransform * NodeTrans;

	//set the transformation that it is going to the shader
	if (m_BoneMapping.find(Nodename) != m_BoneMapping.end()){
		
		GLuint BoneIndex = m_BoneMapping[Nodename];
		
		m_transforms[BoneIndex] = 
									m_userTransform *				//move to the space selected by the user
									m_GlobalInverseTransform *		//move to object space
									GLobaltransform *				//corresponding animation
									m_BoneInfo[BoneIndex].m_Offset	//bone space 
									; 
	}

	//recusrivly move the other children
	for (GLuint i = 0; i < pNode->mNumChildren; i++) {
		BoneHeirarchyTransform(AnimationTime, pNode->mChildren[i], GLobaltransform);
	}
}

glm::mat4 DynamicObject::Interpolatedtransformation(float AnimationTime, const  aiNodeAnim * pNodeAnim){

	glm::quat q;
	glm::vec3 scale;
	glm::vec3 trans;


	//calculate rotation
	if (pNodeAnim->mNumRotationKeys == 1){
		q.x = pNodeAnim->mRotationKeys[0].mValue.x;
		q.y = pNodeAnim->mRotationKeys[0].mValue.y;
		q.z = pNodeAnim->mRotationKeys[0].mValue.z;
		q.w = pNodeAnim->mRotationKeys[0].mValue.w;
	}
	else{
		//search for the correct time
		for (GLuint i = 0; i < pNodeAnim->mNumRotationKeys - 1; ++i){

			//calculate the 
			if (pNodeAnim->mRotationKeys[i + 1].mTime > AnimationTime){
				glm::quat q1;
				q1.x = pNodeAnim->mRotationKeys[i].mValue.x;
				q1.y = pNodeAnim->mRotationKeys[i].mValue.y;
				q1.z = pNodeAnim->mRotationKeys[i].mValue.z;
				q1.w = pNodeAnim->mRotationKeys[i].mValue.w;

				glm::quat q2;
				q2.x = pNodeAnim->mRotationKeys[i + 1].mValue.x;
				q2.y = pNodeAnim->mRotationKeys[i + 1].mValue.y;
				q2.z = pNodeAnim->mRotationKeys[i + 1].mValue.z;
				q2.w = pNodeAnim->mRotationKeys[i + 1].mValue.w;

				float Delta = float(pNodeAnim->mRotationKeys[i + 1].mTime - pNodeAnim->mRotationKeys[i].mTime);
				float t = float((AnimationTime - pNodeAnim->mRotationKeys[i].mTime) / Delta);
				q = q1 * (1.0f - t) + q2 * t;

				//skip other iterations
				i = pNodeAnim->mNumRotationKeys;
			}
		}
			
	}

	

	//calculate scale
	if (pNodeAnim->mNumScalingKeys == 1){
		scale.x = pNodeAnim->mScalingKeys[0].mValue.x;
		scale.y = pNodeAnim->mScalingKeys[0].mValue.y;
		scale.z = pNodeAnim->mScalingKeys[0].mValue.z;
	}
	else{
		//search for the correct time
		for (GLuint i = 0; i < pNodeAnim->mNumScalingKeys - 1; ++i){

			//calculate the 
			if (pNodeAnim->mScalingKeys[i + 1].mTime > AnimationTime){
				glm::vec3 s1;
				s1.x = pNodeAnim->mScalingKeys[i].mValue.x;
				s1.y = pNodeAnim->mScalingKeys[i].mValue.y;
				s1.z = pNodeAnim->mScalingKeys[i].mValue.z;

				glm::vec3 s2;
				s2.x = pNodeAnim->mScalingKeys[i + 1].mValue.x;
				s2.y = pNodeAnim->mScalingKeys[i + 1].mValue.y;
				s2.z = pNodeAnim->mScalingKeys[i + 1].mValue.z;

				float Delta = float(pNodeAnim->mScalingKeys[i + 1].mTime - pNodeAnim->mScalingKeys[i].mTime);
				float t = float((AnimationTime - pNodeAnim->mScalingKeys[i].mTime) / Delta);
				scale = s1 * (1.0f - t) + s2 * t;

				//skip other iterations
				i = pNodeAnim->mNumScalingKeys;
			}
		}

	}


	//calculate translation
	if (pNodeAnim->mNumPositionKeys == 1){
		trans.x = pNodeAnim->mPositionKeys[0].mValue.x;
		trans.y = pNodeAnim->mPositionKeys[0].mValue.y;
		trans.z = pNodeAnim->mPositionKeys[0].mValue.z;
		
	}
	else{
		//search for the correct time
		for (GLuint i = 0; i < pNodeAnim->mNumPositionKeys - 1; ++i){

			//calculate the 
			if (pNodeAnim->mPositionKeys[i + 1].mTime > AnimationTime){
				glm::vec3 trans1;
				trans1.x = pNodeAnim->mPositionKeys[i].mValue.x;
				trans1.y = pNodeAnim->mPositionKeys[i].mValue.y;
				trans1.z = pNodeAnim->mPositionKeys[i].mValue.z;

				glm::vec3 trans2;
				trans2.x = pNodeAnim->mPositionKeys[i + 1].mValue.x;
				trans2.y = pNodeAnim->mPositionKeys[i + 1].mValue.y;
				trans2.z = pNodeAnim->mPositionKeys[i + 1].mValue.z;

				float Delta = float(pNodeAnim->mPositionKeys[i + 1].mTime - pNodeAnim->mPositionKeys[i].mTime);
				float t = float((AnimationTime - pNodeAnim->mPositionKeys[i].mTime) / Delta);
				trans = trans1 * (1.0f - t) + trans2 * t;
				//skip other iterations
				i = pNodeAnim->mNumPositionKeys;
			}
		}
	}
	
	return		glm::translate(glm::mat4(), trans) * 
				glm::mat4_cast(glm::normalize(q))  *
				glm::scale(glm::mat4(), scale);
}


/**
* Render MD2 using Vertex Buffer Object
*/
void DynamicObject::Draw()
{
	for (GLuint i = 0; i < m_vMeshes.size(); ++i){
		m_vMeshes[i].Draw();
	}
}

/**
* Method to Load texture (If any)
*
* @params m_Object a pointer to the md2 structure
* 
*/
/*void DynamicObject::LoadTexture(md2_model_t * m_Object)
{
	if(m_Object->header.num_skins != 0)
	{

		m_pText = new Texture();
		int flo = m_sFile.find_last_of("/");
		if (!m_pText->LoadTexture((m_sFile.substr(0,flo) + "/" + m_Object->skins[0].name).c_str())) {
			delete(m_pText);
			m_pText = NULL;
			m_pText = new Texture();
			m_pText->LoadTexture("Scene/white.png");
		}
	}
	else
	{
		//Load a white texture in case the model doesn't have one
		m_pText = new Texture();
		m_pText->LoadTexture("Scene/white.png");
	}
}*/


/**
* Method to get Bounding Box
*
* @return the bounding box of the model
*/
BoundingBox DynamicObject::GetBoundingBox()
{
	return m_bb;
}


#undef FILE_REVISION

// Revision History:
// $Log: $
// $Header: $
// $Id: $