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

private:
	GLuint VAO, VBO, VBO2;
	GLuint uMVP, uModel;
	vector<glm::vec3> spositions;
	vector<glm::vec3> tpositions;
	vector<glm::vec3> tnormals;
};