// VegaInterpolationRendering.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//#ifdef GLEW_STATIC
//#  define GLEWAPI extern
//#else
//#  ifdef GLEW_BUILD
//#    define GLEWAPI extern __declspec(dllexport)
//#  else
//#    define GLEWAPI extern __declspec(dllimport)
//#  endif
//#endif

#include <iostream>
#include <GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "../RenderingProcess/Camera.h"
#include "../RenderingProcess/Shader.h"
#include "../RenderingProcess/Sence.h"
#include "../RenderingProcess/Mesh.h"
#include "../VegaFemFactory/VegaFemFactory.h"
#include "../RenderingProcess/TreeInstanceMesh.h"
//#include "volumetricMeshLoader.h"
//#include "tetMesh.h"
//#include <vector>
//#include <string>
//#include <cstdio>
//#include <cassert>
//#include <float.h>
//#include "sceneObjectDeformable.h"

bool initWindow(GLFWwindow*& vWindow, int vScreenWidth, int vScreenHeight);
void scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset);
void mouseCallback(GLFWwindow* vWindow, double vXPos, double vYPos);
void framebufferSizeCallback(GLFWwindow* vWindow, int vWidth, int vHeight);
void processInput(GLFWwindow* vWindow);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
CCamera Camera(glm::vec3(0, 0, 0));
float LastX = SCR_WIDTH / 2.0f;
float LastY = SCR_HEIGHT / 2.0f;
bool FirstMouse = true;

// timing
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

//wind处理优化
int windactive = 0;
float grasstime = 0.0f;

int main()
{
	CVegaFemFactory vFem("D:\\GraduationProject\\Vega\\models\\8.10\\test", "D://GraduationProject//Vega//models//8.10//1.obj");
	std::vector<int> b{ 500, 500, 500 };
	std::vector<std::pair<int, int>> angle;
	int numbercounter = 1;
	for (int i = 0; i < numbercounter; i++)
	{
		angle.push_back(std::make_pair(0, i * 30));
	}
	/*std::vector<std::vector<glm::vec3>> u = vFem.objDeformation(a, b);*/
	for (int i = 0; i < numbercounter; i++)
	{
		//给定角度下相关联的一些位移帧文件集合，但由于目前只有一个，就每个角度特定对应一个
		std::vector<Common::SFileFrames> vtemp = vFem.searchFileFrames(angle[i].first, angle[i].second, b);
		vFem.readFramesDeformationData(vtemp, i);//i本来应该是vtemp.size()
	}

	/*std::vector<Common::SFileFrames> vtemp=vFem.searchFileFrames(a.first,a.second, b);
	vFem.readFramesDeformationData(vtemp,0);
	_CrtDumpMemoryLeaks();*/
	// glfw: initialize and configure
	// ------------------------------
	GLFWwindow* Window = nullptr;
	if (!initWindow(Window, SCR_WIDTH, SCR_HEIGHT))
	{
		return EXIT_FAILURE;
	}

	// configure global opengl state
	// -----------------------------
	/*glEnable(GL_CULL_FACE);*/
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// build and compile shaders
	// -------------------------

	CShader ourShader("D:\\GraduationProject\\Vega\\VegaRendering\\VegaInterpolationRendering\\Tree.vert", "D:\\GraduationProject\\Vega\\VegaRendering\\VegaInterpolationRendering\\Tree.frag");

	// load models
	// -----------
	//CModel ourModel("../Model/xiangri/file.obj");

	CSence ourModel("D:/GraduationProject/Vega/models/8.10/1.obj");
	ourModel.setGroupsIndex(vFem);
	ourModel.setVerticesNumber(vFem);
	int i = 0;
	int frame = 0, time = 0, timebase = 0;
	// render loop
	// -----------
	//CTreeInstanceMesh *treeDeformationSet= new  CTreeInstanceMesh[numbercounter];
	CTreeInstanceMesh treeDeformationSet[3];
	std::vector<glm::vec3> DeformationOfFrames;
	for (int j = 0; j < numbercounter; j++)
	{
		//得到每一帧的按照面的顺序排列的u
		/*std::vector<Common::SFileDataGroup> temp = vFem.getConnectedFemMutileDeformation(j, i);
		vFem.cleanSFileDataGroup(j, i);*/
		//CTreeInstanceMesh tempMesh = ourModel.addMeshDeformationForTree(temp);

		treeDeformationSet[j] = CTreeInstanceMesh(ourModel, vFem.getFileFrames(j));
		for (int i = 0; i < vFem.getFileFrames(0).Frames.size(); i++)
		{
			Common::SFileData frame = vFem.getFileFrames(0).Frames[i];
			for (int k = 0; k < frame.BaseFileDeformations.size(); k++)
			{
				DeformationOfFrames.push_back(frame.BaseFileDeformations[k]);
			}
			//DeformationOfFrames = frame.BaseFileDeformations;
		}
		//treeDeformationSet[i].setDeformationFileFrames(vFem.getFileFrames(i));
		//treeDeformationSet[i].__setupMesh
	}

	while (!glfwWindowShouldClose(Window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		DeltaTime = (currentFrame - LastFrame);
		LastFrame = currentFrame;
		std::cout<<i << "帧间隔时间" << DeltaTime << std::endl;
		/*outputfile << deltaTime << endl;*/
		// input
		// -----
		processInput(Window);

		// render 
		// ------
		glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		/*terrainShader.use();*/
		ourShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(Camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = Camera.getViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(1.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		//ourShader.setMat4("model", model);
		//std::vector<Common::SFileDataGroup> temp;
		//ourModel.draw(ourShader, false, temp);

		if (i > 20)
		{
			i = i % 20;
		}
		for (int j = 0; j < numbercounter; j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.2f*j, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			ourShader.setInt("frameIndex", i);
			//每一帧的按照面的顺序排列的u,j=树，i=帧
			/*std::vector<Common::SFileDataGroup> temp = vFem.getConnectedFemMutileDeformation(j, i);
			vFem.cleanSFileDataGroup(j, i);*/
			//ourModel.draw(ourShader, true);
			treeDeformationSet->draw(ourShader);
		}
		i++;

		//ourModel.draw(ourShader);
		//ourModel.draw(ourShader, true, u);
		glfwSwapBuffers(Window);
		glfwPollEvents();

	}
	//outputfile.close();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

//*********************************************************************
//FUNCTION:
bool initWindow(GLFWwindow*& vWindow, int vScreenWidth, int vScreenHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	vWindow = glfwCreateWindow(vScreenWidth, vScreenHeight, "LearnOpenGL", NULL, NULL);
	if (vWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(vWindow);
	glfwSetFramebufferSizeCallback(vWindow, framebufferSizeCallback);
	glfwSetCursorPosCallback(vWindow, mouseCallback);
	glfwSetScrollCallback(vWindow, scrollCallback);
	glfwSetInputMode(vWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed." << std::endl;
		glfwTerminate();
		return false;
	}
	return true;
}

//*********************************************************************
//FUNCTION:
void processInput(GLFWwindow* vWindow)
{
	if (glfwGetKey(vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, true);

	float cameraSpeed = 2.5f * DeltaTime; // adjust accordingly
	if (glfwGetKey(vWindow, GLFW_KEY_W) == GLFW_PRESS)
		Camera.processKeyboard(FORWARD, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_S) == GLFW_PRESS)
		Camera.processKeyboard(BACKWARD, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_A) == GLFW_PRESS)
		Camera.processKeyboard(LEFT, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_D) == GLFW_PRESS)
		Camera.processKeyboard(RIGHT, DeltaTime);
}

//*********************************************************************
//FUNCTION:
void mouseCallback(GLFWwindow* vWindow, double vXPos, double vYPos)
{
	if (FirstMouse)
	{
		LastX = vXPos;
		LastY = vYPos;
		FirstMouse = false;
	}

	float XOffset = vXPos - LastX;
	float YOffset = LastY - vYPos;
	LastX = vXPos;
	LastY = vYPos;

	Camera.processMouseMovement(XOffset, YOffset);
}

//*********************************************************************
//FUNCTION:
void scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset)
{
	Camera.processMouseScroll(vYOffset);
}

//*********************************************************************
//FUNCTION:
void framebufferSizeCallback(GLFWwindow* vWindow, int vWidth, int vHeight)
{
	glViewport(0, 0, vWidth, vHeight);
}

