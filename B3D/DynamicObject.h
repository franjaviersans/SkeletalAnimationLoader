// $Id: $
//
// Author: Francisco Sans franjaviersans@gmail.com
//
// Complete history on bottom of file

#ifndef DynamicObject_H
#define DynamicObject_H



//Includes

#include "Definitions.h"
#include "Vertex.h"
#include "TextureManager.h"
#include "BoundingBox.h"
#include "Mesh.h"
#include <vector>




class BoneInfo{
	public:
		glm::mat4 m_Offset;
};


/**
* Class DynamicObject.
* A Class to track the GPU time from different part of the program
* Also includes a log to print all the times
*
*/
class DynamicObject
{
	//Functions

	public:
		///Default constructor
		DynamicObject(GLfloat velocity, GLuint Width, GLuint Height, GLint start, GLint end);

		///Default destructor
		~DynamicObject();

		///Load an MD2 model from file.
		void Import (const char *filename, const glm::mat4 &TransformationMatrix);

		///Calculate the current frame in animation
		void Animate (float time);

		///Render MD2 using Vertex Buffer Object
		void Draw();	


		///Method to get Bounding Box
		BoundingBox GetBoundingBox();

		std::vector<glm::mat4>& getBonesMatrix(GLuint &count){ count = m_transforms.size(); return m_transforms; }

		

	private:
		
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4 &TransformationMatrix);

		void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4 &TransformationMatrix);

		void BoneHeirarchyTransform(float AnimationTime, const aiNode *pNode, const glm::mat4 & parentTransform);

		glm::mat4 Interpolatedtransformation(float AnimationTime, const aiNodeAnim * pNodeAnim);

	//Variables

	public:
		GLfloat m_fInterpolation, m_fAnimationVelocity;
		std::vector<Mesh> m_vMeshes;
		std::map<std::string, GLuint> m_BoneMapping;
		GLuint m_NumBones;
		std::vector<BoneInfo> m_BoneInfo;
		std::vector<glm::mat4> m_transforms;
		glm::mat4 m_userTransform;
		glm::mat4 m_GlobalInverseTransform;

		/*Texture * m_pText;*/
		std::string m_sFile;
		GLuint m_iVao, m_Vbo, m_iFrame, m_iStart, m_iEnd;;
		BoundingBox m_bb;
		std::string m_directory;
		const aiScene* scene;
		Assimp::Importer importer;
		
};


#endif //DynamicObject_H