#pragma once

#include "GameObject.hpp"

class Ball : public GameObject
{
private:
	glm::vec3 m_velocity{};
	void detectCollision();
	void detectFaceCollision(GameObject* obj);
public:
	Ball();
	const glm::vec3& getVelocity() const { return m_velocity; }
	void setVelocity(glm::vec3 velocity);
	virtual void update();
};

