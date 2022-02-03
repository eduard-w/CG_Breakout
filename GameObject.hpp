#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <map>

#include "Obj3D.hpp"

class GameObject
{
private:
	static std::map<std::string, Obj3D*> s_modelMap;
	static std::map<std::string, GLuint> s_textureMap;
	glm::vec3 m_position{};
	glm::mat4 m_rotation{1.0f};
	glm::vec3 m_scale{};
	glm::mat4 m_transform{1.0f};
	Obj3D* m_mesh;
	GLuint m_textureId;
	GameObject();

public:
	static void cleanUp();
	GameObject(glm::vec3 position = glm::vec3{ 0.0f }, glm::vec3 scale = glm::vec3{ 1.0f }, const std::string modelFileName = "cube.obj", const std::string textureFileName = "mandrill.bmp");
	void updateTransform();
	void draw();
	virtual void update() {};
	
	void setPosition(glm::vec3 position);
	void rotate(float angle, const glm::vec3& axis);
	const glm::vec3& getPosition() const {return m_position;}
	const glm::vec3& getScale() const {return m_scale;}
	const glm::mat4& getTransform() const {return m_transform;}
	const Obj3D& getMeshModel() const;
	const GLuint getTextureId() { return m_textureId; };
};

