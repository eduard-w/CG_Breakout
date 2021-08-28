#pragma once
#include <GLFW/glfw3.h>

class InputManager
{
private:
	InputManager() = delete;
	static float s_mousePosX, s_mousePosY;
	static int s_screenWidth, s_screenHeight;

public:
	static float getMousePosX() { return s_mousePosX; };
	static float getMousePosY() { return s_mousePosY; };
	static int getScreenWidth() { return s_screenWidth; };
	static int getScreenHeight() { return s_screenHeight; };
	static GLFWwindow* init();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
	static void error_callback(int error, const char* description);
};