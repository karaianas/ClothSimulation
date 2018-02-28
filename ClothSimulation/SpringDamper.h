#pragma once
#include "Core.h"
#include "Particle.h"

class SpringDamper {
public: 
	SpringDamper(Particle * p1, Particle * p2);
	void computeForce();
	float getDistance();

	// Check
	void printIDs();

// Switch to private later
public:
	float springConstant, dampingFactor;
	float restLength;
	Particle * P1;
	Particle * P2;
};