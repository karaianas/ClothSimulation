#pragma once
#include "Core.h"

using namespace std;

class Point {
public:
	Point(glm::vec3 windPos);

	void setPos(glm::vec3 curr);
	void update(glm::vec3 delta);
	void draw(GLuint program, glm::mat4 P, glm::mat4 V);
	glm::vec3 getPos();

private:
	GLuint VAO, VBO;
	GLuint uMVP, uModel, uColor;
	vector<glm::vec3> positions;
	glm::mat4 worldM;
};
