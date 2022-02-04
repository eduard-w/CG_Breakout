/*	Eduard Weber
	Konstantin Zehnter
*/

#include <string>

#include "GameObject.hpp"
#include "texture.hpp"
#include <iostream>

std::map<std::string, Obj3D*> GameObject::s_modelMap{};
std::map<std::string, GLuint> GameObject::s_textureMap{};

GameObject::GameObject(glm::vec3 position, glm::vec3 scale, const std::string modelFileName, const std::string textureFileName)
	: m_position{ position }, m_scale{ scale }
{
	auto i_model{ s_modelMap.find(modelFileName) };
	if (i_model == s_modelMap.end()) {
		// mesh not loaded yet -> load
		m_mesh = new Obj3D{ ("resources/" + modelFileName).c_str()};
		s_modelMap.insert({ modelFileName, m_mesh });
	}
	else {
		m_mesh = i_model->second;
	}

	auto i_texture{ s_textureMap.find(textureFileName) };
	if (i_texture == s_textureMap.end()) {
		// texture not loaded yet -> load and bind
		m_textureId = loadBMP_custom(("resources/" + textureFileName).c_str());
		s_textureMap.insert({ textureFileName, m_textureId });
	}
	else {
		m_textureId = i_texture->second;
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

void GameObject::rotate(float angle, const glm::vec3& axis) {
	m_rotation = glm::rotate(m_rotation, angle, axis);
}

const Obj3D& GameObject::getMeshModel() const {
	return *m_mesh;
}

void GameObject::cleanUp()
{
	for (auto i : s_modelMap) {
		delete i.second;
	}
	for (auto i : s_textureMap) {
		glDeleteTextures(1, &i.second);
	}
}