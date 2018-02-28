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
	// Spring only
	glm::vec3 e_ = P2->getPos() - P1->getPos();
	float l = glm::length(e_);
	glm::vec3 e = glm::normalize(e_);
	float f_s = -k_s * (l_0 - l);
	glm::vec3 f_1 = f_s * e;
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
