#include "Particle.h"

using namespace std;

Particle::Particle()
{
	isFixed = false;
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
	if (!isFixed)
	{
		// (1) Compute a
		glm::vec3 accel = (1.0f / mass) * force;

		// (2) New velocity and new positon
		velocity += accel * dt;
		position += velocity * dt;

		// (*) Collision with the ground test (temporary)
		if (position.y < 0.0f)
		{
			//cout << "?" << endl;
			position.y = 0.0f - position.y;
			velocity.y = -0.0001f * velocity.y;
			velocity.x = (1 - 0.8f) * velocity.x;
			velocity.z = (1 - 0.8f) * velocity.z;
		}

	}

	// (3) zero out the force
	force = glm::vec3(0.0f);

	//cout << id << ": " << position.x << " " << position.y << " " << position.z << endl;
	//cout << id << ": " << accel.x << " " << accel.y << " " << accel.z << endl;
	//cout << id << ": " << velocity.x << " " << velocity.y << " " << velocity.z << endl;
} 
