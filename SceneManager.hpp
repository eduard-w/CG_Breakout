#pragma once
#include <vector>
#include "GameObject.hpp"

class SceneManager
{
private:
	SceneManager();
	std::vector<GameObject*> sceneObjects{};
	GameObject* ball;

public:
	// getters
	static SceneManager& getInstance();
	const std::vector<GameObject*>& getAllSceneObjects() const { return sceneObjects; };
	const GameObject& getBall() const { return *ball; };
	
	bool hasWon();
	bool hasLost();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	void createSceneObjects();
	void updateAllSceneObjects();
	~SceneManager();
};

