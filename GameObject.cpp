#include "GameObject.hpp"
#include <string>

GameObject::GameObject(const std::string modelFileName, glm::vec3 position, glm::vec3 scale)
	: m_modelFileName{ modelFileName }, m_position{ position }, m_scale{ scale }
{
	updateTransform();
}

void GameObject::updateTransform() {
	m_transform = glm::mat4{ 1.0f };
	m_transform = glm::translate(m_transform, m_position);
	m_transform *= m_rotation;
	m_transform = glm::scale(m_transform, m_scale);
}

void GameObject::draw() {
	getMeshModel().display();
}

void GameObject::setPosition(glm::vec3 position) {
	m_position = position;
	updateTransform();
}

const Obj3D& GameObject::getMeshModel() const {
	static Obj3D s_model{ ("resources/"+m_modelFileName).c_str() };
	return s_model;
}