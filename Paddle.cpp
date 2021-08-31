#include "Paddle.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"

Paddle::Paddle() : GameObject{ "cube.mesh", glm::vec3{0,0,-14.75}, glm::vec3{3,3,0.2}}
{
}

void Paddle::update() {
	setPosition(glm::vec3{ 
		(InputManager::getMousePosY() / (double)InputManager::getScreenHeight() - 0.5) * 10,
		(InputManager::getMousePosX() / (double)InputManager::getScreenWidth() - 0.5) * 10,
		getPosition().z });
}
