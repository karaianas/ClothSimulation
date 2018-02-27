#pragma once

// Set up GLM
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/type_ptr.hpp"// added for the use of value_ptr()


#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

// Types
//typedef unsigned int uint;

// STD library
#include <vector>
#include <ctype.h>
#include <iostream>
#include <algorithm>

#include <fstream>
#include <string>
#include <sstream>