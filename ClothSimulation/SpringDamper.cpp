#include "SpringDamper.h"

using namespace std;

SpringDamper::SpringDamper(Particle * p1, Particle * p2)
{
	P1 = p1;
	P2 = p2;
	restLength = getDistance();
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
