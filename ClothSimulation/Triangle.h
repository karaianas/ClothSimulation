#pragma once
#include "Core.h"
#include "Particle.h"

using namespace std;

class Triangle {
public:
	Triangle(Particle* p1, Particle* p2, Particle* p3);

	void setParams(float cd, float rho_);
	void computeForce(glm::vec3 &v_a);
	void computeNormal();

	// Check
	void printNormal();

	Particle* P1;
	Particle* P2;
	Particle* P3;
	glm::vec3 N;

	float c_d;
	float rho;
};