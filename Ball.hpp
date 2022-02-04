/*	Eduard Weber
	Konstantin Zehnter
*/

#pragma once

#include "GameObject.hpp"

class Ball : public GameObject
{
private:
	glm::vec3 m_velocity{};
	void calcWorldPositionForVertices(glm::vec3  planeVertices[3], GameObject* obj);
	bool doesBallTouchPlane(glm::vec3  planeVertices[3], glm::vec3& vertexNormal);
	bool doesBallTouchPolygon(glm::vec3  planeVertices[3], glm::vec3& vertexNormal);
	void reboundBall(GameObject* obj, glm::vec3& vertexNormal);
	void copyVertices(glm::vec3  planeVertices[3], const std::vector<glm::vec3>& brickVertices, int i);
	void handleCollisions(GameObject* obj, bool& retflag);
public:
	Ball();
	const glm::vec3& getVelocity() const { return m_velocity; }
	void setVelocity(glm::vec3 velocity);
	virtual void update();
};

