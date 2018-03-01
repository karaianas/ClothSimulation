#pragma once
#include "Core.h"
//#include "Particle.h"
#include "SpringDamper.h"
#include "Triangle.h"

using namespace std;

class ParticleSystem {
public:
	ParticleSystem(int gridSize, glm::vec3 offset);

	void update(float dt);
	void draw(GLuint program, glm::mat4 P, glm::mat4 V);
	void drawInit();

	// Checking functions
	void printSprings();

// Switch to private variables after debugging
public:
	int numParticles;
	vector<Particle>* particles;
	vector<SpringDamper>* springs;
	vector<SpringDamper>* springs2;
	vector<Triangle>* triangles;
	//Particle * P;

private:
	GLuint VAO, VBO;
	GLuint uMVP, uModel;
	vector<glm::vec3> positions;
};