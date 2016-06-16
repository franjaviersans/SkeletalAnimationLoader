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
		DynamicObject(GLfloat velocity, GLuint Width, GLuint Height, GLdouble start, GLdouble end);

		///Default destructor
		~DynamicObject();

		///Load an MD2 model from file.
		void Import (const char *filename, const glm::mat4 &TransformationMatrix);

		///Calculate the current frame in animation
		void Animate (double time);

		///Render MD2 using Vertex Buffer Object
		void Draw();	

		///Method to get Bounding Box
		BoundingBox GetBoundingBox();


		///Method to get the array with all the bone transformations
		std::vector<glm::mat4>& getBonesMatrix(GLuint &count){ count = m_finalBoneTransforms.size(); return m_finalBoneTransforms; }

	private:
		///Process the mesh to obtain the object and bones
		void ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4 &TransformationMatrix, Mesh &data);

		///Process a node in the hierarchy to obtain the mesh
		void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4 &TransformationMatrix);

		///Function to process the bone hierarchy
		void BoneHeirarchyTransform(float AnimationTime, const aiNode *pNode, const glm::mat4 & parentTransform);

		///Function to obtain the interpolated animation in a specific time
		glm::mat4 Interpolatedtransformation(float AnimationTime, const aiNodeAnim * pNodeAnim);

	//Variables

	public:
		GLdouble m_fInterpolation, m_fAnimationVelocity;
		std::vector<Mesh> m_vMeshes;
		std::map<std::string, GLuint> m_BoneMapping;
		GLuint m_NumBones;
		std::vector<glm::mat4> m_originalBoneTransform;
		std::vector<glm::mat4> m_finalBoneTransforms;
		glm::mat4 m_userTransform;
		glm::mat4 m_GlobalInverseTransform;

		/*Texture * m_pText;*/
		std::string m_sFile;
		GLdouble m_fStart, m_fEnd;
		BoundingBox m_bb;
		std::string m_directory;
		const aiScene* scene;
		Assimp::Importer importer;
		
};


#endif //DynamicObject_H