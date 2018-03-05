#include "Window.h"

const char* window_title = "Cloth Simulator";
int Window::width;
int Window::height;

#define VERTEX_SHADER_PATH ".//Shaders//shader.vert"
#define FRAGMENT_SHADER_PATH ".//Shaders//shader.frag"
GLint shaderProgram;

float theta = 0.0f;
float dtheta = 3.14159f / 60;
float radius = 5.0f;
float cylheight = 1.0f;

glm::vec3 cam_pos(radius * sin(theta), cylheight, radius * cos(theta));	
glm::vec3 cam_look_at(0.0f, cylheight, 0.0f);	
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);
glm::mat4 Window::P;
glm::mat4 Window::V;

bool wireframe = false;
bool isTest = false;
bool isDrop = false;
bool isLeft = false;
bool isRight = false;

float totalTime = 0.0f;
glm::vec2 prev_pos;
glm::vec2 prev_pos2;

ParticleSystem* parachute;
ParticleSystem* cloth;
ParticleSystem* ropes;
Plane* ground;
Point* cp;

void Window::initialize_objects()
{
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	vector<float> springConstants;
	springConstants.push_back(4.0f);
	springConstants.push_back(0.001f);
	springConstants.push_back(5.6f);//4.2f
	springConstants.push_back(0.001f);
	springConstants.push_back(1.0f);// 1.0f
	springConstants.push_back(0.001f);
	springConstants.push_back(1.414f);// 1.0f
	springConstants.push_back(0.001f);

	// Parachute demo
	float mass = 0.0001;
	float length = 0.03f;//0.01f
	float c_d = 10.0f;//1.0f
	float rho = 1.23f;
	int step = 10;
	int size = 51;
	parachute = new ParticleSystem();
	parachute->setParams(mass, length, springConstants, c_d, rho, step, step);
	//parachute->createMesh(51, 51 * 2, glm::vec3(-float(size * length) / 2.0f, 1.0f, 0.0f));
	parachute->isZ = true;
	parachute->createMesh(size, size, glm::vec3(-float(size * length) / 2.0f, 7.0f, 0.0f));
	parachute->attachRope();
	parachute->attachBox();

	// Cloth demo
	mass = 0.0001;
	length = 0.01f;//0.01f
	c_d = 1.0f;//1.0f
	rho = 1.23f;
	step = 4;
	size = 51;
	cloth = new ParticleSystem();
	cloth->setParams(mass, length, springConstants, c_d, rho, step, step);
	cloth->createMesh(size, size, glm::vec3(-float(size * length) / 2.0f, 0.5f, 0.0f));

	ground = new Plane(-0.002f);

	// Cursor control
	glm::vec3 windPos = glm::vec3(0.0f, 0.0f, 1.0f);
	cp = new Point(windPos);
	cloth->updateWind(windPos);
	parachute->updateWind(windPos);
	
}

void Window::clean_up()
{
	//delete M;
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	if (isTest)
	{
		float dt = 0.001f;// 0.001f
		totalTime += dt;
		parachute->update(dt);
		cloth->update(dt);
		//ropes->update(dt);
	}
}

void Window::display_callback(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	parachute->draw(shaderProgram, P, V);
	cloth->draw(shaderProgram, P, V);
	//ropes->draw(shaderProgram, P, V);
	ground->draw(shaderProgram, P, V);
	cp->draw(shaderProgram, P, V);

	glfwPollEvents();
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key == GLFW_KEY_A)
		{
			theta -= dtheta;
			cam_pos = glm::vec3(radius * sin(theta), cylheight, radius * cos(theta));
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		if (key == GLFW_KEY_D)
		{
			theta += dtheta;
			cam_pos = glm::vec3(radius * sin(theta), cylheight, radius * cos(theta));
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		if (key == GLFW_KEY_W)
		{
			cylheight += 0.2f;
			cam_pos[1] = cylheight;
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		if (key == GLFW_KEY_S)
		{
			cylheight -= 0.2f;
			cam_pos[1] = cylheight;
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// Play/pause
		if (key == GLFW_KEY_SPACE)
		{
			if (isTest)
				isTest = false;
			else
				isTest = true;
		}

		// Wireframe mode
		if (key == GLFW_KEY_1)
		{
			if (wireframe)
				wireframe = false;
			else
				wireframe = true;
		}

		// Drop cloth
		if (key == GLFW_KEY_2)
		{
			cloth->drop();
			//ropes->drop();
		}

		// Drop cloth
		if (key == GLFW_KEY_3)
		{
			parachute->drop();
		}

	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			double xpos;
			double ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			prev_pos = glm::vec2(xpos, ypos);
			isLeft = true;
		}
		else if (action == GLFW_RELEASE)
		{
			isLeft = false;
			isTest = true;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		prev_pos2 = glm::vec2(xpos, ypos);
		isRight = true;
	}
	else
		isRight = false;

}

glm::vec3 Window::trackball(glm::vec2 point)
{
	glm::vec3 mapped;
	float d;
	mapped.x = (2.0f * point.x - width) / width;
	mapped.y = (height - 2.0f * point.y) / height;
	mapped.z = 0.0f;

	d = glm::length(mapped);
	d = (d < 1.0f) ? d : 1.0f;
	mapped.z = sqrtf(1.001f - d * d);
	mapped = glm::normalize(mapped);

	return mapped;
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (isLeft)
	{
		isTest = false;

		float wx = (2.0f * xpos - width) / width;
		float wy = (height - 2.0f * ypos) / height;

		float wx_ = (2.0f * prev_pos.x - width) / width;
		float wy_ = (height - 2.0f * prev_pos.y) / height;

		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(wx - wx_, wy - wy_, 0.0f));
		glm::mat4 PV = P * V;

		cloth->translate(glm::inverse(PV) * T * P * V);

		prev_pos = glm::vec2(xpos, ypos);
	}
	if (isRight)
	{
		// Calculate angle, axis, and rotation mtx
		glm::vec3 prev_pos_ball = trackball(prev_pos2);
		glm::vec3 curr_pos_ball = trackball(glm::vec2(xpos, ypos));

		float angle = glm::degrees(glm::acos(std::min(1.0f, glm::dot(curr_pos_ball, prev_pos_ball))));
		glm::vec3 axis = glm::normalize(glm::cross(prev_pos_ball, curr_pos_ball));
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle * 0.01f, axis);

		// Point rotation
		glm::mat4 PV = P * V;
		glm::vec4 p = glm::vec4(cp->getPos(), 1.0f);
		cp->setPos(R * p);
		cloth->updateWind(cp->getPos());
		parachute->updateWind(cp->getPos());

		// Model rotation
		//M->rotate(R);

		// Camera rotation
		//cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), -angle * 0.01f, axis) * glm::vec4(cam_pos, 1.0f));
		//cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), -angle * 0.01f, axis) * glm::vec4(cam_up, 1.0f));
		//V = glm::lookAt(cam_pos, cam_look_at, cam_up);

		prev_pos2 = glm::vec2(xpos, ypos);
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	float scalefactor = (yoffset > 0) ? 1.1f : 1 / 1.1f;
	//cam_pos = glm::vec3(glm::scale(glm::mat4(1.0f), glm::vec3(scalefactor)) * glm::vec4(cam_pos, 1.0f));
	if (isRight) 
	{
		glm::vec4 p = glm::vec4(cp->getPos(), 1.0f);
		cp->setPos(scalefactor * p);
		cloth->updateWind(cp->getPos());
		parachute->updateWind(cp->getPos());
	}
	else
	{
		radius *= scalefactor;
		cam_pos = glm::vec3(radius * sin(theta), cylheight, radius * cos(theta));
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}
