// $Id: $
//
// Author: Francisco Sans franjaviersans@gmail.com
//
// Complete history on bottom of file

#ifndef Vertex_H
#define Vertex_H



//Includes
#include "Definitions.h"

#define NumBones 4


/**
* Class Vertex.
* A class that defines a vertex.
*
*/
class Vertex
{
	public:
		glm::vec4 WorldCoord;
		glm::vec3 NormalCoord;
		glm::vec2 TextureCoord;
		GLint BoneID[NumBones];
		GLfloat  BoneWeight[NumBones];
		
		

		//Default Constructor
		Vertex() :	
			WorldCoord(0.0f, 0.0f, 0.0f, 0.0f),
			NormalCoord(0.0f, 0.0f, 0.0f),
			TextureCoord(0.0f, 0.0f)
		{
			for (int i = 0; i < NumBones; ++i){
				BoneID[i] = 0;
				BoneWeight[i] = 0.0f;
			}

		}
};




#endif //Vertex_H