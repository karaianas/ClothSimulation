#include "ParticleSystem.h"

using namespace std;

ParticleSystem::ParticleSystem(int gridSize, glm::vec3 offset)
{
	numParticles = gridSize * gridSize;

	// Create particles
	particles = new vector<Particle>();

	int count = 0;
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			Particle x = Particle();
			x.setParams(0.01f, glm::vec3(float(j) * 0.01f, float(i) * 0.01f, 0) + offset, glm::vec3(0.0f), glm::vec3(0.0f));
			x.id = count;			
			// Fix the top row for now
			if (i == gridSize - 1)
			{
				x.isFixed = true;
			}
			count++;
			particles->push_back(x);
			
		}
	}

	// Create spring-dampers
	springs = new vector<SpringDamper>();
	float k_s1 = 50.0f;
	float k_d1 = 0.1f;
	float k_s2 = 30.0f;
	float k_d2 = 0.1f;

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
				s.setParams(k_s1, k_d1);
				springs->push_back(s);

				if (j != gridSize - 1)
				{
					SpringDamper s(&(particles->at(curr)), &(particles->at(diag)));
					s.setParams(k_s2, k_d2);
					springs->push_back(s);
				}
			}
		
			if (j != gridSize - 1)
			{
				SpringDamper s(&(particles->at(curr)), &(particles->at(right)));
				s.setParams(k_s1, k_d1);
				springs->push_back(s);

				if (i != 0)
				{
					SpringDamper s(&(particles->at(curr)), &(particles->at(diag2)));
					s.setParams(k_s2, k_d2);
					springs->push_back(s);
				}
			}
		}
	}
}

void ParticleSystem::update(float dt)
{
	// (1) Compute gravitational force
	glm::vec3 g(0, -9.8, 0);
	for (int i = 0; i < numParticles; i++)
	{
		glm::vec3 f = g * particles->at(i).getMass();
		particles->at(i).applyForce(f);
	}

	// (2) Compute elastic force
	int count = 0;
	cout << "--------------" << endl;
	for (auto spring : *springs)
	{
		spring.computeForce();
		if (count < 3)
		{
			float dist = spring.getDistance();
			cout << dist << endl;
		}
		count++;
		
	}

	// (3) Integrate
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
