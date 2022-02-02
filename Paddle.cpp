#include "Paddle.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"

Paddle::Paddle() : GameObject{  glm::vec3{0,0,-14}, glm::vec3{3,3,0.2}, "cube.mesh", "mandrill.bmp"}
{
}

float getAdjustedPosX() {
	return (InputManager::getMousePosY() / (double)InputManager::getScreenHeight() - 0.5) * 20;
}

float getAdjustedPosY() {
	return (InputManager::getMousePosX() / (double)InputManager::getScreenWidth() - 0.5) * 20;
}

void Paddle::update() {
	setPosition(glm::vec3{
		getAdjustedPosX() * glm::cos(InputManager::getViewAngle()) - getAdjustedPosY() * glm::sin(InputManager::getViewAngle()),
		getAdjustedPosY() * glm::cos(InputManager::getViewAngle()) + getAdjustedPosX() * glm::sin(InputManager::getViewAngle()),
		getPosition().z });

}
