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
	
	void attachRope();
	void createRope(glm::vec2 indices);

	void attachBox();
	void createBox();

	void drop();
	void update(float dt);
	void draw(GLuint program, glm::mat4 P, glm::mat4 V);
	void drawInit();

	void translate(glm::mat4 T);
	void updateWind(glm::vec3 windPos);

// Switch to private variables after debugging
public:
	int numParticles;
	vector<Particle>* particles;
	vector<SpringDamper>* springs;
	vector<SpringDamper>* springs2;
	vector<Triangle>* triangles;

	bool isZ;

private:
	GLuint VAO, VBO, VBO2, VAO2, VBO3;
	GLuint uMVP, uModel, uColor;

	vector<glm::vec3> spositions;
	vector<glm::vec3> tpositions;
	vector<glm::vec3> tnormals;
	vector<glm::vec3> shadows;

	// attach: attaching points of cloth / attach2: of ropes
	vector<int> attach;
	vector<int> attach2;
	vector<int> attach3;

	glm::vec3 wind;

	// Coefficients
	float k_s1, k_d1, k_s2, k_d2, k_s3, k_d3, k_s4, k_d4;
	float mass, length;
	float c_d, rho;
	int step;
	int dw, dh;
	int w, h;
};