#include "ParticleSystem.h"

void ParticleSystem::update(float dt)
{
	// (1) Compute net force
	glm::vec3 g(0, -9.8, 0);
	for (int i = 0; i < numParticles; i++)
	{
		glm::vec3 f = g * P->at(i).getMass();
		P->at(i).applyForce(f);
	}

	// (2) Integrate
	for (int i = 0; i < numParticles; i++)
		P->at(i).update(dt);
}
