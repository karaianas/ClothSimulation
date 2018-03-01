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
			x.setParams(0.0001f, glm::vec3(float(j) * 0.01f, float(i) * 0.01f, 0) + offset, glm::vec3(0.0f), glm::vec3(0.0f));
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
	triangles = new vector<Triangle>();

	float k_s1 = 4.0f;//60
	float k_d1 = 0.001f;
	float k_s2 = 4.2f;//40
	float k_d2 = 0.001f;// 0.05

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			int p[4] = {-1, -1, -1, -1};
			p[0] = gridSize * i + j;

			int counter = 0;
			if (j < gridSize - 1)
			{
				p[1] = gridSize * i + (j + 1);
				counter++;
			}
			if (i < gridSize - 1)
			{
				p[3] = gridSize * (i + 1) + j;
				counter++;
			}
			if (counter == 2)
				p[2] = gridSize * (i + 1) + (j + 1);

			counter = 0;
			if (p[1] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[1])));
				s.setParams(k_s1, k_d1);
				springs->push_back(s);
				counter++;
			}

			if (p[3] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[3])));
				s.setParams(k_s1, k_d1);
				springs->push_back(s);
				counter++;
			}
			
			if (p[2] > 0)
			{
				// Diagonal spring-dampers
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[2])));
				s.setParams(k_s2, k_d2);
				springs->push_back(s);

				SpringDamper s_(&(particles->at(p[1])), &(particles->at(p[3])));
				s_.setParams(k_s2, k_d2);
				springs->push_back(s_);

				// Triangles
				Triangle t(&(particles->at(p[0])), &(particles->at(p[1])), &(particles->at(p[3])));
				Triangle t_(&(particles->at(p[1])), &(particles->at(p[2])), &(particles->at(p[3])));
				t.setParams(1.0f, 1.23f);
				t_.setParams(1.0f, 1.23f);
				triangles->push_back(t);
				triangles->push_back(t_);
				//t.printNormal();
			}
		}
	}

	// Create secondary spring-dampers
	springs2 = new vector<SpringDamper>();

	float step = 3;
	float k_s3 = 2.0f;//60
	float k_d3 = 0.001f;
	float k_s4 = 2.0f;//40
	float k_d4 = 0.001f;// 0.05

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			int p[4] = { -1, -1, -1, -1 };
			p[0] = gridSize * i + j;

			int counter = 0;
			if (j < gridSize - step)
			{
				p[1] = gridSize * i + (j + step);
				counter++;
			}
			if (i < gridSize - step)
			{
				p[3] = gridSize * (i + step) + j;
				counter++;
			}
			if (counter == 2)
				p[2] = gridSize * (i + step) + (j + step);

			counter = 0;
			if (p[1] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[1])));
				s.setParams(k_s3, k_d3);
				springs->push_back(s);
				counter++;
			}

			if (p[3] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[3])));
				s.setParams(k_s3, k_d3);
				springs->push_back(s);
				counter++;
			}

			if (p[2] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[2])));
				s.setParams(k_s4, k_d4);
				springs->push_back(s);
			}

			if (counter == 2)
			{
				SpringDamper s(&(particles->at(p[1])), &(particles->at(p[3])));
				s.setParams(k_s4, k_d4);
				springs->push_back(s);
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
	for (auto spring : *springs)
		spring.computeForce();

	for (auto spring : *springs2)
		spring.computeForce();

	// (3) Computer aerodynamic force
	for (auto t : *triangles)
		t.computeForce(glm::vec3(0.0f, 0.0f, 5.0f));

	// (3) Integrate
	for (int i = 0; i < numParticles; i++)
		particles->at(i).update(dt);
}

void ParticleSystem::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	/*
	spositions.clear();
	for (auto spring : *springs)
	{
		spositions.push_back(spring.P1->getPos()); 
		spositions.push_back(spring.P2->getPos());
	}
	*/
	tpositions.clear();
	tnormals.clear();
	for (auto t : *triangles)
	{
		tpositions.push_back(t.P1->getPos());
		tpositions.push_back(t.P2->getPos());
		tpositions.push_back(t.P3->getPos());

		t.computeNormal();
		//t.printNormal();
		tnormals.push_back(t.N);
		tnormals.push_back(t.N);
		tnormals.push_back(t.N);
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
	//glDrawArrays(GL_LINES, 0, spositions.size());
	glDrawArrays(GL_TRIANGLES, 0, tpositions.size());
	glBindVertexArray(0);
}

void ParticleSystem::drawInit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * spositions.size(), spositions.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tpositions.size(), tpositions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tnormals.size(), tnormals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

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
