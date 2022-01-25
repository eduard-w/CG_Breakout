/*	Eduard Weber
	Konstantin Zehnter
*/

#include <stdio.h>
#include <vector>

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

// Load the texture
GLuint texture{};

void createTexture() {
	if (!texture)
		texture = loadBMP_custom("resources/mandrill.bmp");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(shaderProgramID, "myTextureSampler"), 0);

	//glDeleteTextures(1, &texture);
}

float calcVectorValue(int brickAmount, int brickNr) {
	float factor = 0.5f * brickAmount - 0.5f;
	return (brickNr - factor) * 4;
}

void createSceneObjects(SceneManager& sceneManager) {
	GameObject* ball{ new Ball };
	sceneManager.addGameObject(ball);
	sceneManager.addGameObject(new Paddle);
	sceneManager.addGameObject(new Frame);
	
	int bricksPerRow = 6;
	int bricksPerColumn = 2;

	for (int x = 0; x < bricksPerRow; x++) {
		for (int y = 0; y < bricksPerColumn; y++) {
			glm::vec3 brickPos = glm::vec3(calcVectorValue(bricksPerRow, x), calcVectorValue(bricksPerColumn, y), 10);
			sceneManager.addGameObject(new Brick{ brickPos });
		}
	}
}

void doGlSubroutines() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void setupMvp() {
	glm::vec3 cameraPos = glm::vec3{ glm::cos(InputManager::getViewAngle())*30, glm::sin(InputManager::getViewAngle())*30, 15};

	projection = glm::perspective(75.0f, 4.0f / 3.0f, 0.1f, 100.0f);
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
	sceneManager.updateAllSceneObjects();

	for (GameObject* o : sceneManager.getAllSceneObjects()) {
		model = o->getTransform();
		syncMvpMatrixWithGpu();
		o->draw();
	}
}

void cleanUp() {
	GameObject::cleanUp();
	glDeleteProgram(shaderProgramID);
	glDeleteTextures(1, &texture);
	glfwTerminate();
}

int main(void)
{
	GLFWwindow* window = InputManager::init();

	shaderProgramID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(shaderProgramID);
	createTexture();
	SceneManager& sceneManager{ SceneManager::getInstance() };
	createSceneObjects(sceneManager);
	glm::vec3 cameraPos{ glm::vec3(60, 0, 20) };

	while (!glfwWindowShouldClose(window))
	{
		doGlSubroutines();
		setupMvp();
		drawEachSceneObject(sceneManager);

		// provide light position to shader program
		glUniform3f(glGetUniformLocation(shaderProgramID, "LightPosition_worldspace"), 0, 0, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		InputManager::frame_callback();
	}

	cleanUp();
	return 0;
}
