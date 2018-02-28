#pragma once
#include "Core.h"

class Particle {
public:
	Particle();
	
	void setParams(float m, glm::vec3 p, glm::vec3 v, glm::vec3 f);
	void update(float dt);
	void draw();
	void applyForce(glm::vec3 &f) { force += f; };

	float getMass() { return mass; };
	glm::vec3 getPos() { return position; };

private:
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;

public:
	int id;
};