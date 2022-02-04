/*	Eduard Weber
	Konstantin Zehnter
*/

#pragma once

void mainLoop(std::chrono::steady_clock::time_point& startTime, SceneManager& sceneManager, GLFWwindow* window, std::chrono::duration<double, std::milli>& runTime, std::chrono::milliseconds& frameRate, int& retflag);
