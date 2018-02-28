#pragma once
#include "Core.h"
#include "Particle.h"

class SpringDamper {
public: 
	SpringDamper(Particle * p1, Particle * p2);

	void setParams(float ks, float kd);
	void computeForce();
	float getDistance();


	// Check
	void printIDs();

// Switch to private later
public:
	float k_s, k_d;
	float l_0;
	Particle * P1;
	Particle * P2;
};