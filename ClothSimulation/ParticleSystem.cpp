#include "ParticleSystem.h"

using namespace std;

ParticleSystem::ParticleSystem(int gridSize, glm::vec3 offset)
{
	numParticles = gridSize * gridSize;

	// Allocate space
	particles = new vector<Particle>();

	// Create particles
	int count = 0;
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			Particle x = Particle();
			x.setParams(0.01f, glm::vec3(float(i) * 0.01f, float(j) * 0.01f, 0) + offset, glm::vec3(0.0f), glm::vec3(0.0f));
			x.id = count;
			count++;
			particles->push_back(x);
		}
	}

	// Create spring-dampers
	springs = new vector<SpringDamper>();

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			int curr = gridSize * i + j;
			int right = gridSize * i + (j + 1);
			int down = gridSize * (i + 1) + j;
			int diag = gridSize * (i + 1) + (j + 1);
			int diag2 = gridSize * (i - 1) + (j + 1);
			
			if (i != gridSize - 1)
			{
				SpringDamper s(&(particles->at(curr)), &(particles->at(down)));
				springs->push_back(s);

				if (j != gridSize - 1)
				{
					SpringDamper s(&(particles->at(curr)), &(particles->at(diag)));
					springs->push_back(s);
				}
			}
		
			if (j != gridSize - 1)
			{
				SpringDamper s(&(particles->at(curr)), &(particles->at(right)));
				springs->push_back(s);

				if (i != 0)
				{
					SpringDamper s(&(particles->at(curr)), &(particles->at(diag2)));
					springs->push_back(s);
				}
			}
		}
	}
}

void ParticleSystem::update(float dt)
{
	// (1) Compute net force
	glm::vec3 g(0, -9.8, 0);
	for (int i = 0; i < numParticles; i++)
	{
		glm::vec3 f = g * particles->at(i).getMass();
		particles->at(i).applyForce(f);
	}

	// (2) Integrate
	for (int i = 0; i < numParticles; i++)
		particles->at(i).update(dt);
}

void ParticleSystem::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	positions.clear();
	for (auto spring : *springs)
	{
		positions.push_back(spring.P1->getPos()); 
		positions.push_back(spring.P2->getPos());
	}
	drawInit();

	glm::mat4 worldM = glm::mat4(1.0f);
	glm::mat4 MVP = P * V * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");

	// Now send these values to the shader program
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 0, positions.size());
	glBindVertexArray(0);
}

void ParticleSystem::drawInit()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::printSprings()
{
	for (auto spring : *springs)
	{
		spring.printIDs();
	}
}
