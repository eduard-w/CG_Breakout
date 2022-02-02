#include "SceneManager.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Frame.hpp"
#include "Brick.hpp"

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

void SceneManager::removeGameObject(GameObject* gameObject)
{
	auto it = std::find(sceneObjects.begin(), sceneObjects.end(), gameObject);
	if (it != sceneObjects.end())
		sceneObjects.erase(it);
	delete gameObject;
}

void SceneManager::createSceneObjects() {
	addGameObject(new Ball);
	addGameObject(new Paddle);
	addGameObject(new Frame);

	int bricksPerRow = 8;
	int bricksPerColumn = 8;

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
}

SceneManager::~SceneManager()
{
	for (GameObject* o : sceneObjects) {
		delete o;
	}
}
