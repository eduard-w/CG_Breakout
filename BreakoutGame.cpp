/*	Eduard Weber
	Konstantin Zehnter
*/

#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "objects.hpp"
#include "objloader.hpp"
#include "texture.hpp"
#include "Obj3D.hpp"
#include "GameObject.hpp"
#include <iostream>
#include "Brick.hpp"
#include "SceneManager.hpp"
#include "InputManager.hpp"
#include "Paddle.hpp"
#include "Frame.hpp"
#include "Ball.hpp"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

GLFWvidmode return_struct;

glm::mat4 model, view, projection;

GLuint shaderProgramID;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

void doGlSubroutines() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void setupMvp() {
	glm::vec3 cameraPos = glm::vec3{ glm::cos(InputManager::getViewAngle())*20, glm::sin(InputManager::getViewAngle())*20, 5};

	projection = glm::perspective(120.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	model = glm::mat4(1.0f);
	glm::mat4 Save = model;
}

void syncMvpMatrixWithGpu()
{
	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "M"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "V"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "P"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
}

void drawEachSceneObject(SceneManager& sceneManager) {

	for (GameObject* o : sceneManager.getAllSceneObjects()) {
		// assign texture and transform
		glActiveTexture(GL_TEXTURE0 + o->getTextureId());
		glBindTexture(GL_TEXTURE_2D, o->getTextureId());
		glUniform1i(glGetUniformLocation(shaderProgramID, "myTextureSampler"), o->getTextureId());
		model = o->getTransform();
		
		syncMvpMatrixWithGpu();
		o->draw();
	}
}

void cleanUp() {
	GameObject::cleanUp();
	glDeleteProgram(shaderProgramID);
	glfwTerminate();
}

int main(void)
{
	GLFWwindow* window = InputManager::init();

	shaderProgramID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(shaderProgramID);
	SceneManager& sceneManager{ SceneManager::getInstance() };
	sceneManager.createSceneObjects();

	// run at 60 frames per second
	int fps = 60;
	std::chrono::milliseconds frameRate{1000/fps};
	std::chrono::steady_clock::time_point startTime;
	std::chrono::duration<double, std::milli> runTime;

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		startTime = std::chrono::high_resolution_clock::now();

		doGlSubroutines();
		setupMvp();
		sceneManager.updateAllSceneObjects();
		drawEachSceneObject(sceneManager);
		
		// win condition
		if (sceneManager.hasWon()) {
			std::cout << "YOU WON\n";
			break;
		}
		
		// fail condition
		if (sceneManager.hasLost()) {
			std::cout << "YOU LOST\n";
			break;
		}

		// provide light position to shader program
		glUniform3f(glGetUniformLocation(shaderProgramID, "LightPosition_worldspace"), 0, 0, -10);

		glfwSwapBuffers(window);
		glfwPollEvents();
		InputManager::frame_callback();

		runTime = std::chrono::high_resolution_clock::now() - startTime;
		if (runTime < frameRate) {
			std::this_thread::sleep_for(frameRate - runTime);
		}
	}

	cleanUp();
	return 0;
}
