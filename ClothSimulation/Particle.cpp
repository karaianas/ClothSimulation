#include "Particle.h"

using namespace std;

Particle::Particle()
{
}

void Particle::setParams(float m, glm::vec3 p, glm::vec3 v, glm::vec3 f)
{
	mass = m;
	position = p;
	velocity = v;
	force = f;
}

void Particle::update(float dt)
{
	// (1) Computer a
	glm::vec3 accel = (1.0f / mass) * force;

	// (2) New velocity and new positon
	velocity += accel * dt;
	position += velocity * dt;

	// (3) zero out the force
	force = glm::vec3(0.0f);

	//cout << id << ": " << position.x << " " << position.y << " " << position.z << endl;
	//cout << id << ": " << accel.x << " " << accel.y << " " << accel.z << endl;
	//cout << id << ": " << velocity.x << " " << velocity.y << " " << velocity.z << endl;
}
