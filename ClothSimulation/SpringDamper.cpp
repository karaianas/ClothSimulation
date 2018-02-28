#include "SpringDamper.h"

using namespace std;

SpringDamper::SpringDamper(Particle * p1, Particle * p2)
{
	P1 = p1;
	P2 = p2;
	l_0 = getDistance();
}

void SpringDamper::setParams(float ks, float kd)
{
	k_s = ks;
	k_d = kd;
}

void SpringDamper::computeForce()
{
	glm::vec3 e_ = P2->getPos() - P1->getPos();
	float l = glm::length(e_);
	glm::vec3 e = glm::normalize(e_);

	// Spring
	float f_s = -k_s * (l_0 - l);

	// Damper
	float v1 = glm::dot(e, P1->getVel());
	float v2 = glm::dot(e, P2->getVel());
	float dv = v1 - v2;
	float f_d = -k_d * dv;

	glm::vec3 f_1 = (f_s + f_d) * e;
	glm::vec3 f_2 = -f_1;

	P1->applyForce(f_1);
	P2->applyForce(f_2);
}

float SpringDamper::getDistance()
{
	glm::vec3 p1 = P1->getPos();
	glm::vec3 p2 = P2->getPos();

	return glm::length(p1 - p2);
}

void SpringDamper::printIDs()
{
	cout << P1->id << " " << P2->id << endl;
}
