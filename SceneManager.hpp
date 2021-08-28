#pragma once
#include <vector>
#include "GameObject.hpp"
class SceneManager
{
private:
	std::vector<GameObject*> sceneObjects{};
	SceneManager();

public:
	static const SceneManager& getInstance();
	void addGameObject(GameObject* gameObject);
	const auto getAllSceneObjects() { return sceneObjects; };
	void updateAllSceneObjects();
	~SceneManager();
};

