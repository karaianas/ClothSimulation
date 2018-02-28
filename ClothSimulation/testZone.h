#pragma once

#include "Core.h"
#include "ParticleSystem.h"

using namespace std;

// Test particles with only gravity
void test01()
{
	// Create particles
	vector<Particle>* P = new vector<Particle>();
	for (int i = 0; i < 1; i++)
	{
		Particle x;
		x.setParams(0.01, glm::vec3(i, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
		x.id = i;
		P->push_back(x);
	}

	// Create particle system
	ParticleSystem PS;
	PS.setParticles(P);

	for (float dt = 0.0f; dt < 1.0f; dt += 0.01f)
	{
		PS.update(0.01);
	}
}