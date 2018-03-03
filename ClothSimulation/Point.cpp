#include "Point.h"

using namespace std;

Point::Point(glm::vec3 windPos)
{
	worldM = glm::mat4(1.0f);
	positions.push_back(windPos);
	positions.push_back(glm::vec3(0.0f));
	positions.push_back(glm::vec3(windPos.x, 0.0f, windPos.z));
	positions.push_back(glm::vec3(0.0f));
}

void Point::setPos(glm::vec3 curr)
{
	positions[0] = curr;
	curr.y = 0.0f;
	positions[2] = curr;
}

void Point::update(glm::vec3 delta)
{
	worldM *= glm::translate(glm::mat4(1.0f), delta);
}

void Point::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw
	glm::mat4 MVP = P * V * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	glUniform3f(uColor, 1.0f, 0.2f, 0.4f);

	glBindVertexArray(VAO);
	glLineWidth(5.0f);
	glDrawArrays(GL_LINES, 0, positions.size());
	glBindVertexArray(0);
}

glm::vec3 Point::getPos()
{
	return positions[0];
}
