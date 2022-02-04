/*	Eduard Weber
	Konstantin Zehnter
*/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <functional>

#include "InputManager.hpp"
#include <iostream>

float InputManager::s_mousePosX{ 0 };
float InputManager::s_mousePosY{ 0 };
int InputManager::s_screenWidth{ 0 };
int InputManager::s_screenHeight{ 0 };
float InputManager::s_viewAngle{ 0 };

GLFWwindow* InputManager::init() {

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

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
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetWindowSize(window, &s_screenWidth, &s_screenHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// init callbacks
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	return window;
}

bool rotateCameraLeft{ false }, rotateCameraRight{ false };
void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool hold{ action != GLFW_RELEASE };

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_A:
		rotateCameraLeft = hold;
		break;
	case GLFW_KEY_D:
		rotateCameraRight = hold;
		break;
	default:
		break;
	}
}

void InputManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	s_mousePosX = xpos;
	s_mousePosY = ypos;
}

void InputManager::error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void InputManager::frame_callback() {
	if (rotateCameraLeft) s_viewAngle -= 0.05;
	if (rotateCameraRight) s_viewAngle += 0.05;
}