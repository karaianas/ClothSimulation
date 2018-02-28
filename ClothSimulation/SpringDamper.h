#pragma once
#include "Core.h"
#include "Particle.h"

class SpringDamper {
public:
	float springConstant, dampingFactor;
	float restLength;
	Particle * P1; 
	Particle * P2;

//public: 
	SpringDamper(Particle * p1, Particle * p2);
	void computeForce();

	// Check
	void printIDs();
};