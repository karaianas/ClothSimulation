#pragma once
#include "Core.h"

class Particle {
public:
	Particle();

	void setParams(float m, glm::vec3 p, glm::vec3 v, glm::vec3 f);
	void update(float dt);
	void draw();

	void applyForce(glm::vec3 &f) { force += f; };
	void addNormal(glm::vec3 &N);
	void computeNormal();

	float getMass() { return mass; };
	glm::vec3 getPos() { return position; };
	glm::vec3 getVel() { return velocity; };
	glm::vec3 getNorm() { return normal; };

	void setPos(glm::vec3 pos) { position = pos; };

private:
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	glm::vec3 normal;

	int ncounter;

public:
	int id;
	bool isFixed;
};