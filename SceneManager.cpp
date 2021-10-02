#include "SceneManager.hpp"

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
