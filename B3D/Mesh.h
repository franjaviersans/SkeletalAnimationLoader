// $Id: $
//
// Author: Francisco Sans franjaviersans@gmail.com
//
// Complete history on bottom of file

#ifndef MESH_H
#define MESH_H



//Includes
#include "Definitions.h"
#include "Vertex.h"
#include <vector>
#include <iostream>


/**
* Class Mesh.
* A class that defines a mesh.
*
*/
class Mesh
{
public:
	std::vector<Vertex> m_vVertexInfo;
	std::vector<GLuint> m_vIndex;
	GLuint m_uiVao;
	GLuint m_uiVbo;
	GLuint m_uiIndexVbo;
	GLuint m_uitextureID;
	GLboolean init;
	GLuint m_iSize;
	std::string m_textureName;


	Mesh() : init(false), m_uiVao(5000), m_uiIndexVbo(5000), m_uiVbo(5000), m_textureName(""){};


	~Mesh(){
		if (init){
			if (m_uiVao != 5000)
			glDeleteVertexArrays(1, &m_uiVao);
			if (m_uiIndexVbo != 5000)
			glDeleteBuffers(1, &m_uiIndexVbo);
			if (m_uiVbo  != 5000)
			glDeleteBuffers(1, &m_uiVbo);
		}
	}

	void setupMesh(){
		init = true;
		glGenBuffers(1, &m_uiVbo);
		glGenBuffers(1, &m_uiIndexVbo);

		// bind buffer for positions and copy data into buffer
		// GL_ARRAY_BUFFER is the buffer type we use to feed attributes
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);
		glBufferData(GL_ARRAY_BUFFER, m_vVertexInfo.size()*sizeof(Vertex), &m_vVertexInfo[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//Set buffer index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexVbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndex.size()*sizeof(GLuint), &m_vIndex[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//Generate the VAO
		glGenVertexArrays(1, &m_uiVao);
		glBindVertexArray(m_uiVao);

		// bind buffer for positions and copy data into buffer
		// GL_ARRAY_BUFFER is the buffer type we use to feed attributes
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexVbo);

		glEnableVertexAttribArray(WORLD_COORD_LOCATION);
		glEnableVertexAttribArray(NORMAL_COORD_LOCATION);
		glEnableVertexAttribArray(TEXTURE_COORD_LOCATION);
		glEnableVertexAttribArray(BONES_ID_LOCATION);
		glEnableVertexAttribArray(BONES_WEIGTH_LOCATION);

		


		glVertexAttribPointer(WORLD_COORD_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)); //Vertexs
		glVertexAttribPointer(NORMAL_COORD_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(GL_FLOAT) * 4)); //Normals
		glVertexAttribPointer(TEXTURE_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(GL_FLOAT) * 7)); //Text Coords
		glVertexAttribIPointer(BONES_ID_LOCATION, 4, GL_INT, sizeof(Vertex), BUFFER_OFFSET(sizeof(GL_FLOAT) * 9));
		glVertexAttribPointer(BONES_WEIGTH_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(GL_FLOAT) * 9 + sizeof(GL_INT) * 4));
		

		//Unbind the vertex array	
		glBindVertexArray(0);


		//Disable Buffers and vertex attributes
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		m_iSize = m_vIndex.size();

		m_vVertexInfo.clear();
		m_vIndex.clear();
	}

	void Draw(){

		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->BindTexture(m_uitextureID);

		// Draw mesh
		glBindVertexArray(m_uiVao);
		glDrawElements(GL_TRIANGLES, m_iSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

};




#endif //Vertex_H