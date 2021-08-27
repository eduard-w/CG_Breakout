#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Obj3D.hpp"
class GameObject
{
private:
	glm::vec3 m_position{};
	glm::mat4 m_rotation{1.0f};
	glm::vec3 m_scale{};
	glm::mat4 m_transform{1.0f};
	Obj3D& m_model;

public:
	GameObject(Obj3D& model, glm::vec3 position = glm::vec3{ 0.0f }, glm::vec3 scale = glm::vec3{ 1.0f });
	void updateTransform();
	void draw();
	const glm::vec3& getPosition() {return m_position;}
	void setPosition(glm::vec3 position);
	const glm::mat4& getTransform() {return m_transform;}
};

