/*	Eduard Weber
	Konstantin Zehnter
*/

#pragma once
#include <vector>
#include "GameObject.hpp"

class SceneManager
{
private:
	SceneManager();
	std::vector<GameObject*> sceneObjects{};
	std::vector<int> toBeRemovedObjects{};
	GameObject* ball;

public:
	// getters
	static SceneManager& getInstance();
	const std::vector<GameObject*>& getAllSceneObjects() const { return sceneObjects; };
	const GameObject& getBall() const { return *ball; };
	
	bool hasWon();
	bool hasLost();
	void addGameObject(GameObject* gameObject);
	void scheduleRemoveGameObject(GameObject* gameObject);
	void removeAllScheduledGameObjects();
	void createSceneObjects();
	void updateAllSceneObjects();
	~SceneManager();
};

