#pragma once
#include "Core.h"
//#include "Particle.h"
#include "SpringDamper.h"
#include "Triangle.h"

using namespace std;

class ParticleSystem {
public:
	ParticleSystem();

	void setParams(float m, float len, vector<float> springC, float drag, float den, int stepw, int steph);
	void createMesh(int width, int height, glm::vec3 offset);
	void createRope(int ropeSize, glm::vec3 offset);

	void drop();
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
	GLuint uMVP, uModel, uColor;
	vector<glm::vec3> spositions;
	vector<glm::vec3> tpositions;
	vector<glm::vec3> tnormals;

	// Coefficients
	float k_s1, k_d1, k_s2, k_d2, k_s3, k_d3, k_s4, k_d4;
	float mass, length;
	float c_d, rho;
	int step;
	int dw, dh;
	int w, h;
};