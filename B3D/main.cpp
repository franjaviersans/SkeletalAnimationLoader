// Includes
#include "Definitions.h"
#include <iostream>
#include <fstream>
using namespace std;
#include "DynamicObject.h"
#include "GLSLProgram.h"
#include "TextureManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using std::cout;
using std::endl;



namespace glfwFunc
{
	GLFWwindow* glfwWindow;
	int WINDOW_WIDTH = 1024;
	int WINDOW_HEIGHT = 768;
	std::string strNameWindow = "Compute Shader Volume Ray Casting";

	const float NCP = 1.0f;
	const float FCP = 10.0f;
	const float fAngle = 45.f * (3.14f / 180.0f); //In radians

	float color[] = { 1, 1, 1 };
	bool pintar = false;

	GLSLProgram m_program;
	glm::mat4x4 mProjMatrix, mModelViewMatrix, mMVP;


	int inicio = 40, fin = 60;
	DynamicObject * dynamicObj = NULL;


	///< Callback function used by GLFW to capture some possible error.
	void errorCB(int error, const char* description)
	{
		printf("%s\n", description);
	}



	///
	/// The keyboard function call back
	/// @param window id of the window that received the event
	/// @param iKey the key pressed or released
	/// @param iScancode the system-specific scancode of the key.
	/// @param iAction can be GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	/// @param iMods Bit field describing which modifier keys were held down (Shift, Alt, & so on)
	///
	void keyboardCB(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMods)
	{
		/*if (iAction == GLFW_PRESS)
		{
			switch (iKey)
			{
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_SPACE:
				g_pTransferFunc->isVisible = !g_pTransferFunc->isVisible;
				break;
			}
		}*/
		/* Escape */
		/*if (key == 27)
			exit(0);
		switch (key)
		{
		case '1':
			//Animate (0, md2fileheader.num_frames - 1, &n, &interp);
			xxx++;
			break;
		case '2':
			xxx--;
			break;
		case '3':
			yyy++;
			md2file->m_iEnd++;
			break;
		case '4':
			yyy--;
			md2file->m_iEnd--;
			break;
		case '5':
			zzz++;
			break;
		case '6':
			zzz--;
			break;
		case 't':
			// imdebugTexImagef(GL_TEXTURE_2D, obj->GetTextureAtlas()->GetTextId(),GL_RGB,0,"rgb b=16f");
			break;
		case 'u':
			break;
		}*/
		// printf("inicio:%d fin:%d\n",md2file->m_iStart,md2file->m_iEnd);
	}

	inline int TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos)
	{

		
		return true;
	}

	int TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods)
	{
		

		return true;
	}

	///< The resizing function
	void resizeCB(GLFWwindow* window, int iWidth, int iHeight)
	{

		WINDOW_WIDTH = iWidth;
		WINDOW_HEIGHT = iHeight;

		if (iHeight == 0) iHeight = 1;
		float ratio = iWidth / float(iHeight);
		glViewport(0, 0, iWidth, iHeight);

		mProjMatrix = glm::perspective(float(fAngle), ratio, NCP, FCP);
	}



	///< The main rendering function.
	void draw()
	{
		GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "INICIO " << err << std::endl;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw a Cube
		m_program.use();
		{

			mModelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));

			m_program.setUniform("Projection", mProjMatrix);
			m_program.setUniform("modelView", mModelViewMatrix);


			

			/*md2file->Animate(double(0));
			md2file->UpdateVAO();*/

			//glFrontFace(GL_CW);
			dynamicObj->Draw();
			//glFrontFace(GL_CCW);
		}

		glfwSwapBuffers(glfwWindow);
	}


	///
	/// Init all data and variables.
	/// @return true if everything is ok, false otherwise
	///
	bool initialize()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		//Init GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			printf("- glew Init failed :(\n");
			return false;
		}
		printf("OpenGL version: %s\n", glGetString(GL_VERSION));
		printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));


		// send window size events to AntTweakBar
		glfwSetWindowSizeCallback(glfwWindow, resizeCB);
		glfwSetMouseButtonCallback(glfwWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
		glfwSetCursorPosCallback(glfwWindow, (GLFWcursorposfun)TwEventMousePosGLFW3);
		glfwSetKeyCallback(glfwWindow, (GLFWkeyfun)keyboardCB);


		//load the shaders
		try{
			m_program.compileShader(string("Normal.vert"), GLSLShader::VERTEX);
			m_program.compileShader(string("Normal.frag"), GLSLShader::FRAGMENT);
			m_program.link();
		}
		catch (GLSLProgramException & e) {
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}


		dynamicObj = new DynamicObject(1.0f, 512, 512, 0, 15);


		//glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f))*glm::scale(glm::vec3(0.02, 0.02, 0.02))

		//glm::scale(glm::vec3(0.208715f)) * glm::translate(glm::mat4(), glm::vec3(0.0f, -4.79286f, 1.71198))
		
		//dynamicObj->Import("Model/warrior.md2", glm::scale(glm::vec3(0.208715f)) * glm::translate(glm::mat4(), glm::vec3(0.0f, -4.79286f, 1.71198)));

		dynamicObj->Import("Model/Apple.off", glm::scale(glm::vec3(0.006)) * glm::translate(glm::mat4(), glm::vec3(-8,13,-12.4)));
		//dynamicObj->Import("Model/Apple.off", glm::mat4(1.0f));

		cout << dynamicObj->GetBoundingBox().m_fMinx << " " << dynamicObj->GetBoundingBox().m_fMiny << " " << dynamicObj->GetBoundingBox().m_fMinz << endl <<
			"  " << dynamicObj->GetBoundingBox().m_fMaxx << " " << dynamicObj->GetBoundingBox().m_fMaxy << " " << dynamicObj->GetBoundingBox().m_fMaxz << endl;


		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(
			(dynamicObj->GetBoundingBox().m_fMaxx + dynamicObj->GetBoundingBox().m_fMinx) / 2.0f,
			(dynamicObj->GetBoundingBox().m_fMaxy + dynamicObj->GetBoundingBox().m_fMiny) / 2.0f,
			(dynamicObj->GetBoundingBox().m_fMaxz + dynamicObj->GetBoundingBox().m_fMinz) / 2.0f)
									);

		float s = 1.0f / max((dynamicObj->GetBoundingBox().m_fMaxx - dynamicObj->GetBoundingBox().m_fMinx)/2.0f,
			max((dynamicObj->GetBoundingBox().m_fMaxy - dynamicObj->GetBoundingBox().m_fMiny) / 2.0f,
			(dynamicObj->GetBoundingBox().m_fMaxz - dynamicObj->GetBoundingBox().m_fMinz) / 2.0f));

		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(s));

		cout << s << endl;
		cout << (dynamicObj->GetBoundingBox().m_fMaxx + dynamicObj->GetBoundingBox().m_fMinx) / 2.0f << "  " << 
			(dynamicObj->GetBoundingBox().m_fMaxy + dynamicObj->GetBoundingBox().m_fMiny) / 2.0f << "  " << 
			(dynamicObj->GetBoundingBox().m_fMaxz + dynamicObj->GetBoundingBox().m_fMinz) / 2.0f << endl;

		glm::mat4 trans2 = glm::translate(glm::mat4(), glm::vec3(0.0f,0.0f, -3.0f));

		mModelViewMatrix = trans2 * scale * -trans;


		mModelViewMatrix = trans2;

		return true;
	}


	/// Here all data must be destroyed + glfwTerminate
	void destroy()
	{
		TextureManager::Inst()->UnloadAllTextures();
		if (dynamicObj) dynamicObj->~DynamicObject();
		glfwTerminate();
		glfwDestroyWindow(glfwWindow);
	}
}

int main(int argc, char** argv)
{

	glfwSetErrorCallback(glfwFunc::errorCB);
	if (!glfwInit())	exit(EXIT_FAILURE);
	glfwFunc::glfwWindow = glfwCreateWindow(glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT, glfwFunc::strNameWindow.c_str(), NULL, NULL);
	if (!glfwFunc::glfwWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(glfwFunc::glfwWindow);
	if (!glfwFunc::initialize()) exit(EXIT_FAILURE);
	glfwFunc::resizeCB(glfwFunc::glfwWindow, glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT);	//just the 1st time;


	while (!glfwWindowShouldClose(glfwFunc::glfwWindow))
	{

		glfwFunc::draw();
		glfwPollEvents();	//or glfwWaitEvents()
	}


	glfwFunc::destroy();
	return EXIT_SUCCESS;
	}





/*
//
// Animate model from frames 0 to num_frames-1 
md2file->Animate(double(curent_time - last_time));
md2file->UpdateVAO();
;




/* Load MD2 model file 
md2file->Import(filename, glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f))*glm::scale(glm::vec3(0.02, 0.02, 0.02)));
*/