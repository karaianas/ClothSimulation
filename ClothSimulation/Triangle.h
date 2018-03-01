#pragma once
#include "Core.h"
#include "Particle.h"

using namespace std;

class Triangle {
public:
	Triangle(Particle* p1, Particle* p2, Particle* p3);

	void computeArea();
	void computeNormal();

	// Check
	void printNormal();

	Particle* P1;
	Particle* P2;
	Particle* P3;
	glm::vec3 N;
};