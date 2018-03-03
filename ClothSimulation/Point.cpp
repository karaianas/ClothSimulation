#include "Point.h"

using namespace std;

Point::Point()
{
	worldM = glm::mat4(1.0f);
	//worldM *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f));
}

void Point::setPos(glm::vec3 curr)
{
	worldM[3] = glm::vec4(curr, 1.0f);
}

void Point::update(glm::vec3 delta)
{
	worldM *= glm::translate(glm::mat4(1.0f), delta);
}

void Point::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	// Rebinding
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO); 

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw
	//glm::mat4 worldM = glm::mat4(1.0f);
	glm::mat4 MVP = P * V * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	glUniform3f(uColor, 1.0f, 0.7f, 0.4f);

	glBindVertexArray(VAO);
	glPointSize(8.0f);
	//cout << positions[0].x << " " << positions[0].y << " " << positions[0].z << endl;
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

glm::vec3 Point::getPos()
{
	return glm::vec3(worldM[3]);
}
