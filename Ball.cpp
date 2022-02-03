#include "Ball.hpp"
#include "SceneManager.hpp"
#include "Brick.hpp"
#include "Paddle.hpp"

#include <typeinfo>
#include <iostream>

const int VERTICES_PER_POLYGON = 3;

Ball::Ball() : GameObject{ glm::vec3{0.0f,0.0f,-10.0f}, glm::vec3{0.5f}, "ball.obj", "mandrill.bmp"}
{
	setVelocity(glm::normalize(glm::vec3{ 0.2f, 0.3f, 0.2f }) * 0.3f);
}

void Ball::setVelocity(glm::vec3 velocity) {
	m_velocity = velocity;
}

void Ball::calcWorldPositionForVertices(glm::vec3  planeVertices[3], GameObject* obj)
{
	planeVertices[0] *= obj->getScale();
	planeVertices[1] *= obj->getScale();
	planeVertices[2] *= obj->getScale();
	planeVertices[0] += obj->getPosition();
	planeVertices[1] += obj->getPosition();
	planeVertices[2] += obj->getPosition();
}

bool Ball::doesBallTouchPlane(glm::vec3  planeVertices[3], glm::vec3& vertexNormal)
{
	glm::vec3 vertex1ToBall = getPosition() - planeVertices[0];
	float distance = -glm::dot(vertex1ToBall, vertexNormal);
	bool outsidePlane = false;
	float radius = getScale().x / 2;
	return fabs(distance) <= radius;
}

static bool leftOf(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p)
{
	float area = 0.5f * (a.x * (b.y - p.y) +
		b.x * (p.y - a.y) +
		p.x * (a.y - b.y));
	return (area > -0.5f);
}

bool Ball::doesBallTouchPolygon(glm::vec3  planeVertices[3], glm::vec3& vertexNormal)
{
	glm::vec3 edgeOfPlane = planeVertices[1] - planeVertices[0];
	glm::vec3 planeX{ glm::normalize(edgeOfPlane) };
	glm::vec3 planeY{ glm::normalize(glm::cross(vertexNormal, edgeOfPlane)) };

	auto project2D = [&](const glm::vec3& p) {
		return glm::vec2{ glm::dot(p, planeX), glm::dot(p, planeY) };
	};

	glm::vec2 v1_2D{ project2D(planeVertices[0]) };
	glm::vec2 v2_2D{ project2D(planeVertices[1]) };
	glm::vec2 v3_2D{ project2D(planeVertices[2]) };
	glm::vec2 ball_2D{ project2D(getPosition()) };

	return leftOf(v1_2D, v2_2D, ball_2D) && leftOf(v2_2D, v3_2D, ball_2D) && leftOf(v3_2D, v1_2D, ball_2D);
}

void Ball::reboundBall(GameObject* obj, glm::vec3& vertexNormal)
{
	if (typeid(*obj) == typeid(Paddle)) {
		glm::vec3 newBallDirection = getPosition() - (obj->getPosition() - glm::vec3(0, 0, 1));
		setVelocity(glm::normalize(newBallDirection) * 0.3f);
	}
	else {
		setVelocity(getVelocity() - 2 * glm::dot(getVelocity(), vertexNormal) * vertexNormal);
	}
	if (typeid(*obj) == typeid(Brick)) {
		SceneManager::getInstance().scheduleRemoveGameObject(obj);
	}
	return;
}

void Ball::copyVertices(glm::vec3  planeVertices[3], const std::vector<glm::vec3>& brickVertices, int i)
{
	planeVertices[0] = brickVertices[i];
	planeVertices[1] = brickVertices[i + 1];
	planeVertices[2] = brickVertices[i + 2];
}

void Ball::handleCollisions(GameObject* obj, bool& retflag) {
	retflag = true;
	const auto brickVertices = obj->getMeshModel().getVertices();
	const auto brickNormals = obj->getMeshModel().getNormals();
	
	for (int i = 0; i <  brickVertices.size(); i+=VERTICES_PER_POLYGON) {
		glm::vec3 planeVertices[VERTICES_PER_POLYGON];
		glm::vec3 vertexNormal = glm::normalize(brickNormals[i]);
		
		copyVertices(planeVertices, brickVertices, i);
		calcWorldPositionForVertices(planeVertices, obj);

		if (!doesBallTouchPlane(planeVertices, vertexNormal)) continue;
		if (doesBallTouchPolygon(planeVertices, vertexNormal)) return reboundBall(obj, vertexNormal);
	}
	retflag = false;
}

void Ball::update() {
	setPosition(getPosition() + getVelocity());
	rotate(5, getVelocity());

	for (GameObject* obj : SceneManager::getInstance().getAllSceneObjects()) {
		if (typeid(*obj) == typeid(Ball))
			continue;

		bool retflag;
		handleCollisions(obj, retflag);
		if (retflag) return;
	}
}