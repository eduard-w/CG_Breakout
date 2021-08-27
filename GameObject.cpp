#include "GameObject.hpp"

GameObject::GameObject(Obj3D& model, glm::vec3 position, glm::vec3 scale)
	: m_model{ model }, m_position{ position }, m_scale{ scale }
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
	m_model.display();
}

void GameObject::setPosition(glm::vec3 position) {
	m_position = position;
	updateTransform();
}