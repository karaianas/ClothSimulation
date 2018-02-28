#pragma once
#include "Core.h"
#include "Particle.h"

using namespace std;

class ParticleSystem {
	int numParticles;
	vector<Particle>* P;
	//Particle * P;

public:
	void setParticles(vector<Particle>* p) { P = p; numParticles = p->size(); };
	void update(float dt);
	void draw();
};