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
		DynamicObject(GLfloat velocity, GLuint Width, GLuint Height, GLint start, GLint end);

		///Default destructor
		~DynamicObject();

		///Load an MD2 model from file.
		void Import (const char *filename, const glm::mat4 &TransformationMatrix);

		///Calculate the current frame in animation
		void Animate (float time);

		///Render MD2 using Vertex Buffer Object
		void Draw();

		///Update Vertex Buffer Object
		void UpdateVAO();		

		///Get Texture Width
		GLuint GetTextureWidth();

		///Get Texture Height
		GLuint GetTextureHeight();

		///Method to get Bounding Box
		BoundingBox GetBoundingBox();

	private:
		
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4 &TransformationMatrix);

		void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4 &TransformationMatrix);

	//Variables

	public:
		GLfloat m_fInterpolation, m_fAnimationVelocity;
		std::vector<Mesh> m_vMeshes;

		/*Texture * m_pText;*/
		std::string m_sFile;
		GLuint m_iWidth, m_iHeight, m_iVao, m_Vbo, m_iFrame, m_iStart, m_iEnd;;
		BoundingBox m_bb;
		std::string m_directory;

};


#endif //DynamicObject_H