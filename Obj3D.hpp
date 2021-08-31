#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Obj3D
{
private:
	GLuint VertexArrayID;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint normalbuffer; 
	GLuint uvbuffer;

public:
	Obj3D(const char* fn);
	void display() const;
	~Obj3D();
};

