#pragma once

#include "GameObject.hpp"

class Ball : public GameObject
{
private:
	glm::vec3 m_velocity{};
public:
	Ball();
	const glm::vec3& getVelocity() { return m_velocity; }
	void setVelocity(glm::vec3 velocity);
	virtual void update();
};

