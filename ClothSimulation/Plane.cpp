#include "Plane.h"

using namespace std;

Plane::Plane(float h)
{
	positions.push_back(glm::vec3(-10, h, 10));
	positions.push_back(glm::vec3(10, h, 10));
	positions.push_back(glm::vec3(-10, h, -10));

	positions.push_back(glm::vec3(10, h, 10));
	positions.push_back(glm::vec3(10, h, -10));
	positions.push_back(glm::vec3(-10, h, -10));

	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	height = h;

	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Plane::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	glm::mat4 worldM = glm::mat4(1.0f);
	glm::mat4 MVP = P * V * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	//glUniform3f(uColor, 0.8f, 0.9f, 0.8f);
	glUniform3f(uColor, 0.2f, 0.2f, 0.2f);

	glBindVertexArray(VAO);
	glLineWidth(1.0f);
	glDrawArrays(GL_TRIANGLES, 0, positions.size());
	glBindVertexArray(0);
}
