#include "Ball.hpp"
#include "SceneManager.hpp"
#include "Brick.hpp"
#include "Paddle.hpp"

#include <typeinfo>
#include <iostream>

Ball::Ball() : GameObject{ glm::vec3{0.0f,0.0f,-10.0f}, glm::vec3{0.5f}, "ball.mesh", "mandrill.bmp"}
{
	setVelocity(glm::vec3{ 0.2f, 0.3f, 0.2f });
}

void Ball::setVelocity(glm::vec3 velocity) {
	m_velocity = velocity;
}

static float lengthSquared(glm::vec3 v) {
	return powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2);
}

static bool leftOf(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p)
{
	float area = 0.5f * (a.x * (b.y - p.y) +
		b.x * (p.y - a.y) +
		p.x * (a.y - b.y));
	return (area > -0.5f);
}

void Ball::handleCollisions(GameObject* obj, bool& retflag)
{
	retflag = true;
	const auto brickVertices = obj->getMeshModel().getVertices();
	const auto brickNormals = obj->getMeshModel().getNormals();

	int faceCount = brickVertices.size() / 3;

	for (int i = 0; i < faceCount; i++) {
		if (typeid(*obj) == typeid(Paddle))
			;
		else {
			bool outsidePlane = false;
			bool outsideAllVerts = false;
			bool outsideAllEdges = false;
			bool fullyInsidePlane = false;

			glm::vec3 v1{ brickVertices[i * 3] };
			glm::vec3 v2{ brickVertices[i * 3 + 1] };
			glm::vec3 v3{ brickVertices[i * 3 + 2] };

			// model -> world
			// object rotation is ignored for now
			v1 *= obj->getScale();
			v2 *= obj->getScale();
			v3 *= obj->getScale();
			v1 += obj->getPosition();
			v2 += obj->getPosition();
			v3 += obj->getPosition();
			// in this case all three vertex normals for face are parallel so just take the first one
			glm::vec3 vn = glm::normalize(brickNormals[i * 3]);

			float distance = -glm::dot(getPosition() - v1, vn);
			// scale is same across all axis for ball -> use any
			float radius = getScale().x / 2;
			outsidePlane = fabs(distance) > radius;
			if (outsidePlane) continue;

			glm::vec3 planeX{ glm::normalize(v2 - v1) };
			glm::vec3 planeY{ glm::normalize(glm::cross(vn, v2 - v1)) };
			auto project2D = [&](const glm::vec3& p) {
				return glm::vec2{ glm::dot(p, planeX), glm::dot(p, planeY) };
			};

			glm::vec2 v1_2D{ project2D(v1) };
			glm::vec2 v2_2D{ project2D(v2) };
			glm::vec2 v3_2D{ project2D(v3) };
			glm::vec2 ball_2D{ project2D(getPosition()) };

			if (leftOf(v1_2D, v2_2D, ball_2D) && leftOf(v2_2D, v3_2D, ball_2D) && leftOf(v3_2D, v1_2D, ball_2D)) {
				// inside polygon -> collision detected
				// std::cout << "COLLISION";
				// ball rebounds
				setVelocity(getVelocity() - 2 * glm::dot(getVelocity(), vn) * vn);
				// destroy if brick
				if (typeid(*obj) == typeid(Brick)) {
					SceneManager::getInstance().removeGameObject(obj);
				}
				return;
			}
		}
	}
	retflag = false;
}

void Ball::update() {
	setPosition(getPosition() + getVelocity());
	rotate(5, getVelocity());

	for (GameObject* obj : SceneManager::getInstance().getAllSceneObjects()) {
		// exclude self from scene objects
		if (typeid(*obj) == typeid(Ball))
			continue;

		bool retflag;
		handleCollisions(obj, retflag);
		if (retflag) return;
	}
}