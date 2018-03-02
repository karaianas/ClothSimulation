#include "ParticleSystem.h"

using namespace std;

ParticleSystem::ParticleSystem()
{
}

void ParticleSystem::setParams(float m, float len, vector<float> springC, float drag, float den, int stepw, int steph)
{
	k_s1 = springC[0];
	k_d1 = springC[1];
	k_s2 = springC[2];
	k_d2 = springC[3];
	k_s3 = springC[4];
	k_d3 = springC[5];
	k_s4 = springC[6];
	k_d4 = springC[7];

	mass = m;
	length = len;
	c_d = drag;
	rho = den;
	dw = stepw;
	dh = steph;
}

void ParticleSystem::createMesh(int width, int height, glm::vec3 offset)
{
	numParticles = width * height;
	w = width;
	h = height;

	// Create particles
	particles = new vector<Particle>();

	int count = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Particle x = Particle();
			if (((i == 0) || (i == height - 1)) && ((j == 0) || (j == 1) || (j == width - 2) || (j == width - 1)))
			{
				//x.setParams(mass * 10.0f, glm::vec3(float(j) * length, 0, float(i) * length) + offset, glm::vec3(0.0f), glm::vec3(0.0f));
				attach.push_back(count);
			}
			x.setParams(mass, glm::vec3(float(j) * length, float(i) * length, 0 ) + offset, glm::vec3(0.0f), glm::vec3(0.0f));
			x.id = count;

			// Fix the top row initially
			if (i == height - 1)
			{
				x.isFixed = true;
			}
			//x.isFixed = true;
			count++;
			particles->push_back(x);
		}
	}

	// Create spring-dampers
	springs = new vector<SpringDamper>();
	triangles = new vector<Triangle>();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int p[4] = {-1, -1, -1, -1};
			p[0] = width * i + j;

			int counter = 0;
			if (j < width - 1)
			{
				p[1] = width * i + (j + 1);
				counter++;
			}
			if (i < height - 1)
			{
				p[3] = width * (i + 1) + j;
				counter++;
			}
			if (counter == 2)
				p[2] = width * (i + 1) + (j + 1);

			counter = 0;
			if (p[1] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[1])));
				s.setParams(k_s1, k_d1);
				springs->push_back(s);
				counter++;
			}

			if (p[3] > 0)
			{
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[3])));
				s.setParams(k_s1 , k_d1);
				springs->push_back(s);
				counter++;
			}
			
			if (p[2] > 0)
			{
				// Diagonal spring-dampers
				SpringDamper s(&(particles->at(p[0])), &(particles->at(p[2])));
				s.setParams(k_s2, k_d2);
				springs->push_back(s);

				SpringDamper s_(&(particles->at(p[1])), &(particles->at(p[3])));
				s_.setParams(k_s2, k_d2);
				springs->push_back(s_);

				// Triangles
				Triangle t(&(particles->at(p[0])), &(particles->at(p[1])), &(particles->at(p[3])));
				Triangle t_(&(particles->at(p[1])), &(particles->at(p[2])), &(particles->at(p[3])));
				t.setParams(c_d, rho);
				t_.setParams(c_d, rho);
				triangles->push_back(t);
				triangles->push_back(t_);
				//t.printNormal();
			}
		}
	}

	// Create secondary spring-dampers
	springs2 = new vector<SpringDamper>();
	int wth = 0;
	if (dw <= 0 || dh <= 0)
	{
		//cout << "???" << endl;
		return;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int p[4] = { -1, -1, -1, -1 };
			p[0] = width * i + j;

			//std::cout << "j = " << j << " i = " << i << endl;
			//std::cout << "width - dw = " << width - dw << " height - dh = " << height - dh << endl;

			if ((j <= width - dw) && (i <= height - dh))
			{
				p[1] = width * i + (j + dw - 1);
				p[2] = width * (i + dh - 1) + (j + dw - 1);
				p[3] = width * (i + dh - 1) + j;

				//std::cout << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << endl;

				// Top
				SpringDamper s0(&(particles->at(p[3])), &(particles->at(p[2])));
				s0.setParams(k_s3, k_d3);
				springs->push_back(s0);

				// Bottom
				SpringDamper s1(&(particles->at(p[0])), &(particles->at(p[1])));
				s1.setParams(k_s3, k_d3);
				springs->push_back(s1);

				// Diag
				SpringDamper s2(&(particles->at(p[0])), &(particles->at(p[2])));
				s2.setParams(k_s4, k_d4);
				springs->push_back(s2);

				// Diag
				SpringDamper s3(&(particles->at(p[1])), &(particles->at(p[3])));
				s3.setParams(k_s4, k_d4);
				springs->push_back(s3);

				// Left
				SpringDamper s4(&(particles->at(p[0])), &(particles->at(p[3])));
				s4.setParams(k_s3, k_d3);
				springs->push_back(s4);

				// Right
				SpringDamper s5(&(particles->at(p[1])), &(particles->at(p[2])));
				s5.setParams(k_s3, k_d3);
				springs->push_back(s5);
				wth += 6;
			}
		}
	}
}

void ParticleSystem::attachRope()
{
	for (int i = 0; i < attach.size() - 1; i += 2)
		createRope(glm::vec2(attach[i], attach[i + 1]));
}

void ParticleSystem::createRope(glm::vec2 indices)
{
	int hr = 50;
	int wr = 2;
	float mr = 0.001f;
	float lr = 0.01f;

	float k_s1r = 300.0f;
	float k_d1r = 0.01f;
	float k_s2r = 300.2f;
	float k_d2r = 0.01f;
	float k_s3r = 20.0f;
	float k_d3r = 0.001f;
	float k_s4r = 100.0f;
	float k_d4r = 0.001f;

	float c_dr = c_d;// *0.1f;

	int dwr = 2;
	int dhr = 3;

	glm::vec3 p1 = particles->at(indices[0]).getPos();
	glm::vec3 p2 = particles->at(indices[1]).getPos();

	int count = particles->size();
	vector<Particle>* rparticles = new vector<Particle>();

	for (int i = 0; i < hr; i++)
	{
		Particle x = Particle();
		glm::vec3 pos1 = p1 - glm::vec3(0.0f, float(i + 1) * lr, 0.0f);
		//pos1.x *= float(hr - i) / float(hr);
		//pos1.z *= float(hr - i) / float(hr);
		x.setParams(mr, pos1, glm::vec3(0.0f), glm::vec3(0.0f));
		x.id = count;
		count++;
		rparticles->push_back(x);

		Particle y = Particle();
		glm::vec3 pos2 = p2 - glm::vec3(0.0f, float(i + 1) * lr, 0.0f);
		//pos2.x *= float(hr - i) / float(hr);
		//pos2.z *= float(hr - i) / float(hr);
		y.setParams(mr, pos2, glm::vec3(0.0f), glm::vec3(0.0f));
		y.id = count;
		count++;
		rparticles->push_back(y);

		numParticles += 2;

		if (i == hr - 1)
		{
			attach2.push_back(x.id);
			attach2.push_back(y.id);
		}
	}

	vector<glm::vec3> tid;
	vector<glm::vec3> sid;

	for (int i = 0; i < hr; i++)
	{
		for (int j = 0; j < wr; j++)
		{
			int p[4] = { -1, -1, -1, -1 };
			p[0] = wr * i + j;

			int t0, t1, t2, t3;
			t0 = rparticles->at(p[0]).id;

			int counter = 0;
			if (j < wr - 1)
			{
				p[1] = wr * i + (j + 1);
				t1 = rparticles->at(p[1]).id;
				counter++;
			}
			if (i < hr - 1)
			{
				p[3] = wr * (i + 1) + j;
				t3 = rparticles->at(p[3]).id;
				counter++;
			}
			if (counter == 2)
			{
				p[2] = wr * (i + 1) + (j + 1);
				t2 = rparticles->at(p[2]).id;
			}

			if (p[1] > 0)
				sid.push_back(glm::vec3(0, t0, t1));

			if (p[3] > 0)
				sid.push_back(glm::vec3(0, t0, t3));

			if (p[2] > 0)
			{
				// Springs
				sid.push_back(glm::vec3(1, t0, t2));
				sid.push_back(glm::vec3(1, t1, t3));

				// Triangles
				tid.push_back(glm::vec3(t0, t1, t3));
				tid.push_back(glm::vec3(t1, t2, t3));
			}
		}
	}


	// Create secondary spring-dampers
	//if (dwr <= 0 || dhr <= 0)
	//	return;

	vector<glm::vec3> sid2;

	for (int i = 0; i < hr; i++)
	{
		for (int j = 0; j < wr; j++)
		{
			int p[4] = { -1, -1, -1, -1 };
			p[0] = wr* i + j;

			int t0, t1, t2, t3;
			t0 = rparticles->at(p[0]).id;

			if ((j <= wr - dwr) && (i <= hr - dhr))
			{
				p[1] = wr * i + (j + dwr - 1);
				p[2] = wr * (i + dhr - 1) + (j + dwr - 1);
				p[3] = wr * (i + dhr - 1) + j;

				t1 = rparticles->at(p[1]).id;
				t2 = rparticles->at(p[2]).id;
				t3 = rparticles->at(p[3]).id;

				// Top
				sid2.push_back(glm::vec3(0, t3, t2));

				// Bottom
				sid2.push_back(glm::vec3(0, t0, t1));

				// Diag
				sid2.push_back(glm::vec3(1, t0, t2));

				// Diag
				sid2.push_back(glm::vec3(1, t1, t3));

				// Left
				sid2.push_back(glm::vec3(0, t0, t3));

				// Right
				sid2.push_back(glm::vec3(0, t1, t2));
			}
		}
	}

	for (auto p : *rparticles)
		particles->push_back(p);

	for (int i = 0; i < tid.size(); i++)
	{
		int t1, t2, t3;
		t1 = tid[i].x;
		t2 = tid[i].y;
		t3 = tid[i].z;
		Triangle t(&(particles->at(t3)), &(particles->at(t2)), &(particles->at(t1)));
		t.setParams(c_dr, rho);
		triangles->push_back(t);
	}

	for (int i = 0; i < sid.size(); i++)
	{
		int s1, s2, s3;
		s1 = sid[i].x;
		s2 = sid[i].y;
		s3 = sid[i].z;

		if (s1 < 1)
		{
			SpringDamper s(&(particles->at(s2)), &(particles->at(s3)));
			s.setParams(k_s1r, k_d1r);
			springs->push_back(s);
		}
		else
		{
			SpringDamper s(&(particles->at(s2)), &(particles->at(s3)));
			s.setParams(k_s2r, k_d2r);
			springs->push_back(s);
		}
	}

	for (int i = 0; i < sid2.size(); i++)
	{
		int s1, s2, s3;
		s1 = sid2[i].x;
		s2 = sid2[i].y;
		s3 = sid2[i].z;

		if (s1 < 1)
		{
			SpringDamper s(&(particles->at(s2)), &(particles->at(s3)));
			s.setParams(k_s3r, k_d3r);
			springs->push_back(s);
		}
		else
		{
			SpringDamper s(&(particles->at(s2)), &(particles->at(s3)));
			s.setParams(k_s4r, k_d4r);
			springs->push_back(s);
		}
	}

	// Connecting springs
	int A, B, a, b;
	A = particles->at(indices[0]).id;
	B = particles->at(indices[1]).id;
	a = rparticles->at(0).id;
	b = rparticles->at(1).id;

	SpringDamper sa(&(particles->at(A)), &(particles->at(a)));
	sa.setParams(k_s3r, k_d3r);
	springs->push_back(sa);

	SpringDamper sb(&(particles->at(B)), &(particles->at(b)));
	sb.setParams(k_s3r, k_d3r);
	springs->push_back(sb);

	SpringDamper sAb(&(particles->at(A)), &(particles->at(b)));
	sAb.setParams(k_s4r, k_d4r);
	springs->push_back(sAb);

	SpringDamper saB(&(particles->at(a)), &(particles->at(B)));
	saB.setParams(k_s4r, k_d4r);
	springs->push_back(saB);

	// Triangles
	Triangle abA(&(particles->at(a)), &(particles->at(b)), &(particles->at(A)));
	abA.setParams(c_d, rho);
	triangles->push_back(abA);

	Triangle bBA(&(particles->at(b)), &(particles->at(B)), &(particles->at(A)));
	bBA.setParams(c_d, rho);
	triangles->push_back(bBA);
}

void ParticleSystem::attachBox()
{
	createBox();

	vector<int> rearr;
	rearr.push_back(attach3[3]);
	rearr.push_back(attach3[2]);
	rearr.push_back(attach3[0]);
	rearr.push_back(attach3[1]);

	for (int i = 0; i < attach3.size(); i++)
	{
		int c1 = attach2[2 * i];
		int c2 = attach2[2 * i + 1];
		int b = rearr[i];

		// Create triangle
		Triangle t(&(particles->at(c1)), &(particles->at(b)), &(particles->at(c2)));
		t.setParams(c_d, rho);
		triangles->push_back(t);

		// Create springs
		SpringDamper s1(&(particles->at(c1)), &(particles->at(b)));
		s1.setParams(40.0f, 0.0f);
		s1.l_0 = 0.01f;
		SpringDamper s2(&(particles->at(c2)), &(particles->at(b)));
		s2.l_0 = 0.01f;
		s2.setParams(40.0f, 0.0f);

		springs->push_back(s1);
		springs->push_back(s2);
	}
}

void ParticleSystem::createBox()
{
	//glm::vec3 offset(0.0f, 2.0f, 0.0f);
	glm::vec3 avg(0.0f);
	for (int i = 0; i < attach2.size(); i ++)
	{
		int id = attach2[i];
		avg += particles->at(id).getPos();
	}
	avg /= float(attach2.size());
	avg -= glm::vec3(0.0f, 0.1f, 0.0f);

	vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(-1, -1, 1));
	vertices.push_back(glm::vec3(1, -1, 1));
	vertices.push_back(glm::vec3(1, -1, -1));
	vertices.push_back(glm::vec3(-1, -1, -1));
	vertices.push_back(glm::vec3(-1, 1, 1));
	vertices.push_back(glm::vec3(1, 1, 1));
	vertices.push_back(glm::vec3(1, 1, -1));
	vertices.push_back(glm::vec3(-1, 1, -1));

	vector<int> pid;
	float scaleFactor = 0.1f;
	float len1 = 2.0f * scaleFactor;
	float len2 = 2.0f * sqrt(2.0f) * scaleFactor;

	// Create particles
	int count = particles->size();
	for (int i = 0; i < vertices.size(); i++)
	{
		Particle x = Particle();
		x.setParams(0.1f, vertices[i] * scaleFactor + avg, glm::vec3(0.0f), glm::vec3(0.0f));
		x.id = count;
		particles->push_back(x);
		pid.push_back(count);
		count++;

		if (i > 3)
			attach3.push_back(x.id);
	}

	// Create springs
	int scounter = 0;
	float ks = 100.0f;//300.0f
	float kd = 0.1f;
	for(int i = 0; i < pid.size(); i++)
	{
		for (int j = i + 1; j < pid.size(); j++)
		{
			SpringDamper s(&(particles->at(pid[i])), &(particles->at(pid[j])));
			float distance = glm::length(particles->at(pid[i]).getPos() - particles->at(pid[j]).getPos());
			s.setParams(ks * distance/len1, kd * distance / len1);
			springs->push_back(s);
			scounter++;
		}
	}
	//cout << "springs: " << scounter << endl;

	// Create triangles
	// Near
	Triangle t015(&(particles->at(pid[0])), &(particles->at(pid[1])), &(particles->at(pid[5])));
	Triangle t054(&(particles->at(pid[0])), &(particles->at(pid[5])), &(particles->at(pid[4])));

	// Right
	Triangle t126(&(particles->at(pid[1])), &(particles->at(pid[2])), &(particles->at(pid[6])));
	Triangle t165(&(particles->at(pid[1])), &(particles->at(pid[6])), &(particles->at(pid[5])));

	// Far
	Triangle t237(&(particles->at(pid[2])), &(particles->at(pid[3])), &(particles->at(pid[7])));
	Triangle t276(&(particles->at(pid[2])), &(particles->at(pid[7])), &(particles->at(pid[6])));

	// Left
	Triangle t347(&(particles->at(pid[3])), &(particles->at(pid[4])), &(particles->at(pid[7])));
	Triangle t304(&(particles->at(pid[3])), &(particles->at(pid[0])), &(particles->at(pid[4])));

	// Top
	Triangle t467(&(particles->at(pid[4])), &(particles->at(pid[6])), &(particles->at(pid[7])));
	Triangle t456(&(particles->at(pid[4])), &(particles->at(pid[5])), &(particles->at(pid[6])));

	// Bottom
	Triangle t310(&(particles->at(pid[3])), &(particles->at(pid[1])), &(particles->at(pid[0])));
	Triangle t321(&(particles->at(pid[3])), &(particles->at(pid[2])), &(particles->at(pid[1])));

	t015.setParams(c_d, rho);
	t054.setParams(c_d, rho);
	t126.setParams(c_d, rho);
	t165.setParams(c_d, rho);
	t237.setParams(c_d, rho);
	t276.setParams(c_d, rho);
	t347.setParams(c_d, rho);
	t304.setParams(c_d, rho);
	t467.setParams(c_d, rho);
	t456.setParams(c_d, rho);
	t310.setParams(c_d, rho);
	t321.setParams(c_d, rho);

	triangles->push_back(t015);
	triangles->push_back(t054);
	triangles->push_back(t126);
	triangles->push_back(t165);
	triangles->push_back(t237);
	triangles->push_back(t276);
	triangles->push_back(t347);
	triangles->push_back(t304);
	triangles->push_back(t467);
	triangles->push_back(t456);
	triangles->push_back(t310);
	triangles->push_back(t321);

}

void ParticleSystem::drop()
{
	//for (int i = w * (h - 1); i < numParticles; i++)
	//	particles->at(i).isFixed = false;
	//for (auto p : *particles)
	//{
	//	p.isFixed = false;
	//}
	for (int i = 0; i < particles->size(); i++)
	{
		particles->at(i).isFixed = false;
	}
}

void ParticleSystem::update(float dt)
{

	// (1) Compute gravitational force
	glm::vec3 g(0, -9.8, 0);
	for (int i = 0; i < particles->size(); i++)
	{
		glm::vec3 f = g * particles->at(i).getMass();
		particles->at(i).applyForce(f);
	}

	// (2) Compute elastic force
	for (auto spring : *springs)
		spring.computeForce();

	for (auto spring : *springs2)
		spring.computeForce();

	// (3) Compute aerodynamic force
	for (auto t : *triangles)
		t.computeForce(glm::vec3(0.0f, 0.0f, 5.0f));//5

	// (4) Integrate
	for (int i = 0; i < particles->size(); i++)
		particles->at(i).update(dt);

}

void ParticleSystem::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	//spositions.clear();
	//for (auto p : *particles)
	//{
	//	spositions.push_back(p.getPos());
	//}
	//for (auto spring : *springs)
	//{
	//	spositions.push_back(spring.P1->getPos()); 
	//	spositions.push_back(spring.P2->getPos());
	//}

	tpositions.clear();
	tnormals.clear();
	for (auto t : *triangles)
	{
		tpositions.push_back(t.P1->getPos());
		tpositions.push_back(t.P2->getPos());
		tpositions.push_back(t.P3->getPos());

		t.computeNormal();
		t.P1->addNormal(t.N);
		t.P2->addNormal(t.N);
		t.P3->addNormal(t.N);

		//tnormals.push_back(t.N);
		//tnormals.push_back(t.N);
		//tnormals.push_back(t.N);
	}

	// Smooth normal
	for (auto t : *triangles)
	{
		t.P1->computeNormal();
		t.P2->computeNormal();
		t.P3->computeNormal();

		tnormals.push_back(t.P1->getNorm());
		tnormals.push_back(t.P2->getNorm());
		tnormals.push_back(t.P3->getNorm());
	}

	drawInit();

	glm::mat4 worldM = glm::mat4(1.0f);
	glm::mat4 MVP = P * V * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	glUniform3f(uColor, 0.4f, 0.7f, 1.0f);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_POINTS, 0, spositions.size());
	//glDrawArrays(GL_POINTS, 0, tpositions.size());
	glDrawArrays(GL_TRIANGLES, 0, tpositions.size());
	glBindVertexArray(0);
}

void ParticleSystem::drawInit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * spositions.size(), spositions.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tpositions.size(), tpositions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tnormals.size(), tnormals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::printSprings()
{
	for (auto spring : *springs)
	{
		spring.printIDs();
	}
}
