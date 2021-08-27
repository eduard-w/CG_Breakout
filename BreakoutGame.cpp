#include <stdio.h>
#include <stdlib.h>
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

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

GLFWvidmode return_struct;

float posX{0};
float posY{0};
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_W:
		posX -= 0.1f;
		break;
	case GLFW_KEY_A:
		posY -= 0.1f;
		break;
	case GLFW_KEY_S:
		posX += 0.1f;
		break;
	case GLFW_KEY_D:
		posY += 0.1f;
		break;
	default:
		break;
	}
}

int height{};
int width{};
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	posX = (ypos/(double)height - 0.5)*5;
	posY = (xpos/(double)width - 0.5)*5;
}

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// reference to shader program
GLuint programID;

// sync mvp matrix with gpu
void sendMVP()
{
	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
}

GLuint VertexArrayIDTeapot = 0;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

// Load the texture
GLuint texture{};

void createTexture() {
	if (!texture)
		texture = loadBMP_custom("mandrill.bmp");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

	//glDeleteTextures(1, &texture);
}

void drawCS() {

	glm::mat4 Save = model;
	model = glm::scale(model, glm::vec3(2, 0.02f, 0.02f));
	sendMVP();
	drawCube();
	model = Save;

	model = glm::scale(model, glm::vec3(0.02f, 2, 0.02f));
	sendMVP();
	drawCube();
	model = Save;

	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 2));
	sendMVP();
	drawCube();
	model = Save;
}

void drawSeg(float h) {
	glm::mat4 Save = model;
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, h));
	sendMVP();
	drawSphere(10, 10);
	model = Save;
}

int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "CG - Breakout", NULL, NULL);


	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetWindowSize(window, &width, &height);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(programID);

	createTexture();
	
	std::vector<GameObject*> sceneObjects{};
	Obj3D o_platform{ "cube.obj" };
	Obj3D o_teapot{ "teapot.obj" };
	GameObject teapot{ o_teapot, glm::vec3{0}, glm::vec3{0.001f} };
	GameObject platform{ o_platform, glm::vec3{0,0,-2}, glm::vec3{1, 1, 0.1f} };
	sceneObjects.push_back(&teapot);
	sceneObjects.push_back(&platform);
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	
		// setup mvp
		projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		view = glm::lookAt(glm::vec3(8, 0, 0), 
			glm::vec3(0, 0, 0),  
			glm::vec3(0, 0, 1));
		model = glm::mat4(1.0f);
		glm::mat4 Save = model;

		platform.setPosition(glm::vec3{posX,posY,-2});
		
		// draw each object in scene
		for (GameObject* o : sceneObjects) {
			model = o->getTransform();
			sendMVP();
			o->draw();
		}

		model = Save;
		model = glm::translate(model, glm::vec3(0, 0, 1));
		glm::vec4 lightPos = model * glm::vec4(0, 0, 0, 1);

		// provide light position to shader program
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(programID);
	glDeleteTextures(1, &texture);
	glfwTerminate();
	return 0;
}
