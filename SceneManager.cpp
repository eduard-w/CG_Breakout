/*	Eduard Weber
	Konstantin Zehnter
*/

#include "SceneManager.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Frame.hpp"
#include "Brick.hpp"
#include <typeinfo>
#include <iostream>

SceneManager::SceneManager()
{
}

SceneManager& SceneManager::getInstance()
{
	static SceneManager sceneManager;
	return sceneManager;
}

void SceneManager::addGameObject(GameObject* gameObject)
{
	sceneObjects.push_back(gameObject);
}

void SceneManager::scheduleRemoveGameObject(GameObject* gameObject)
{
	auto it = std::find(sceneObjects.begin(), sceneObjects.end(), gameObject);
	if (it != sceneObjects.end())
		toBeRemovedObjects.push_back(it - sceneObjects.begin());
}

void SceneManager::removeAllScheduledGameObjects() {
	for (int it: toBeRemovedObjects) {
		sceneObjects.erase(sceneObjects.begin() + it);
	}
	toBeRemovedObjects.clear();
}

void SceneManager::createSceneObjects() {
	ball = new Ball;
	addGameObject(ball);
	addGameObject(new Paddle);
	addGameObject(new Frame);

	int bricksPerRow = 4;
	int bricksPerColumn = 4;

	auto calcVectorValue{ [](int brickAmount, int brickNr) -> float {
		float factor = 0.5f * brickAmount - 0.5f;
		return (brickNr - factor) * 4;
	} };

	for (int x = 0; x < bricksPerRow; x++) {
		for (int y = 0; y < bricksPerColumn; y++) {
			glm::vec3 brickPos = glm::vec3(calcVectorValue(bricksPerRow, x), calcVectorValue(bricksPerColumn, y), 10);
			addGameObject(new Brick{ brickPos });
		}
	}
}

void SceneManager::updateAllSceneObjects()
{
	for (GameObject* o : sceneObjects) {
		o->update();
	}
	removeAllScheduledGameObjects();
}

bool SceneManager::hasWon() {
	auto isBrick = [](GameObject* o) {return typeid(*o) == typeid(Brick);};
	return std::find_if(sceneObjects.begin(), sceneObjects.end(), isBrick) == sceneObjects.end();
}

bool SceneManager::hasLost() {
	return getBall().getPosition().z < -14.5f;
}

SceneManager::~SceneManager()
{
	for (GameObject* o : sceneObjects) {
		delete o;
	}
}
