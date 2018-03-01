#include "Triangle.h"

using namespace std;

Triangle::Triangle(Particle * p1, Particle * p2, Particle * p3)
{
	P1 = p1;
	P2 = p2;
	P3 = p3;

	computeNormal();
}

void Triangle::computeNormal()
{
	glm::vec3 A = glm::normalize(P2->getPos() - P1->getPos());
	glm::vec3 B = glm::normalize(P3->getPos() - P1->getPos());
	N = glm::cross(A, B);
}

void Triangle::printNormal()
{
	cout << N.x << " " << N.y << " " << N.z << endl;
}
