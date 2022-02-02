#pragma once
#include <vector>
#include "GameObject.hpp"

class SceneManager
{
private:
	std::vector<GameObject*> sceneObjects{};
	SceneManager();
	float calcVectorValue(int brickAmount, int brickNr);

public:
	static SceneManager& getInstance();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	const auto getAllSceneObjects() const { return sceneObjects; };
	void createSceneObjects();
	void updateAllSceneObjects();
	~SceneManager();
};

