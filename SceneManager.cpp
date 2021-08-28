#include "SceneManager.hpp"

SceneManager::SceneManager()
{
}

const SceneManager& SceneManager::getInstance()
{
	static SceneManager sceneManager;
	return sceneManager;
}

void SceneManager::addGameObject(GameObject* gameObject)
{
	sceneObjects.push_back(gameObject);
}

void SceneManager::updateAllSceneObjects()
{
	for (GameObject* o : sceneObjects) {
	}
}

SceneManager::~SceneManager()
{
	for (GameObject* o : sceneObjects) {
		delete o;
	}
}
