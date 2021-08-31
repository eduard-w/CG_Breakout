#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "Obj3D.hpp"
#include <map>

class GameObject
{
private:
	static std::map<std::string, Obj3D*> s_modelMap;
	glm::vec3 m_position{};
	glm::mat4 m_rotation{1.0f};
	glm::vec3 m_scale{};
	glm::mat4 m_transform{1.0f};
	Obj3D* m_mesh;
	GameObject();

public:
	static void cleanUp();
	GameObject(const std::string modelFileName, glm::vec3 position = glm::vec3{ 0.0f }, glm::vec3 scale = glm::vec3{ 1.0f });
	void updateTransform();
	void draw();
	virtual void update() {};
	const glm::vec3& getPosition() {return m_position;}
	void setPosition(glm::vec3 position);
	const glm::mat4& getTransform() {return m_transform;}
	const Obj3D& getMeshModel() const;
};

