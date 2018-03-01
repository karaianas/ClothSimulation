#pragma once
#include "Core.h"

using namespace std;

class Plane {
public:

	Plane(float h);
	
	void draw(GLuint program, glm::mat4 P, glm::mat4 V);

private:
	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	GLuint VAO, VBO, VBO2;
	GLuint uMVP, uModel;
	float height;
};