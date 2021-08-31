#include <string>

#include "GameObject.hpp"
#include <iostream>

std::map<std::string, Obj3D*> GameObject::s_modelMap{};

GameObject::GameObject(const std::string modelFileName, glm::vec3 position, glm::vec3 scale)
	: m_position{ position }, m_scale{ scale }
{
	auto i_model{ s_modelMap.find(modelFileName) };
	if (i_model == s_modelMap.end()) {
		// mesh not loaded yet -> load
		m_mesh = new Obj3D{ ("resources/" + modelFileName).c_str() };
		s_modelMap.insert({ modelFileName, m_mesh });
	}
	else {
		m_mesh = i_model->second;
	}
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
	return *m_mesh;
}

void GameObject::cleanUp()
{
	for (auto i : s_modelMap) {
		std::cout << "unloading resources/" << i.first << "...\n";
		delete i.second;
	}
}