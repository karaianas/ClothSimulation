#pragma once

#include "Core.h"
#include "ParticleSystem.h"
#include "Plane.h"

ParticleSystem ps;// (31, glm::vec3(0.0f, 0.0f, 0.0f));
Plane ground(0.0f);

using namespace std;

// Test particles with only gravity
void test01()
{
	vector<float> springConstants;
	springConstants.push_back(4.0f);
	springConstants.push_back(0.001f);
	springConstants.push_back(4.2f);
	springConstants.push_back(0.001f);
	springConstants.push_back(2.0f);
	springConstants.push_back(0.001f);
	springConstants.push_back(2.0f);
	springConstants.push_back(0.001f);

	float mass = 0.0001;
	float length = 0.01f;
	float c_d = 1.0f;
	float rho = 1.23f;

	ps.setParams(mass, length, springConstants, c_d, rho);
	ps.createMesh(31, glm::vec3(0.0f, 0.0f, 0.0f));
}