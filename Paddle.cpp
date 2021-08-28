#include "Paddle.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"

Paddle::Paddle() : GameObject{ "cube.obj", glm::vec3{0,0,-2}, glm::vec3{1,1,0.1}}
{
}

void Paddle::update() {
	setPosition(glm::vec3{ 
		(InputManager::getMousePosY() / (double)InputManager::getScreenHeight() - 0.5) * 5,
		(InputManager::getMousePosX() / (double)InputManager::getScreenWidth() - 0.5) * 5,
		-2 });
}
