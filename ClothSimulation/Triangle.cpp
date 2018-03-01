#include "Triangle.h"

using namespace std;

Triangle::Triangle(Particle * p1, Particle * p2, Particle * p3)
{
	P1 = p1;
	P2 = p2;
	P3 = p3;

	computeNormal();
}

void Triangle::setParams(float cd, float rho_)
{
	c_d = cd;
	rho = rho_;
}

void Triangle::computeForce(glm::vec3 &v_a)
{
	glm::vec3 v_s = (P1->getVel() + P2->getVel() + P3->getVel()) / 3.0f;
	glm::vec3 v = v_s - v_a;

	//computeNormal();

	float a_0 = 0.5f * glm::length(glm::cross(P2->getPos() - P1->getPos(), P3->getPos() - P1->getPos()));
	float a; 
	if (glm::length(v) == 0.0f)
		a = 0.0f;
	else
		a = a_0 * glm::dot(v, N) / glm::length(v);

	glm::vec3 f = -0.5f * rho * glm::length(v) * glm::length(v) * c_d * a * N / 3.0f;
	
	//cout << a << endl;
	//cout << v.x << " " << v.y << " " << v.z << endl;
	//cout << f.x << " " << f.y << " " << f.z << endl;
	P1->applyForce(f);
	P2->applyForce(f);
	P3->applyForce(f);

}

void Triangle::computeNormal()
{
	glm::vec3 A = glm::normalize(P2->getPos() - P1->getPos());
	glm::vec3 B = glm::normalize(P3->getPos() - P1->getPos());
	N = glm::normalize(glm::cross(A, B));
}

void Triangle::printNormal()
{
	cout << N.x << " " << N.y << " " << N.z << endl;
}
