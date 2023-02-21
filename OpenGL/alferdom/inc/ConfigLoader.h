#pragma once

#include <GLFW/glfw3.h>
#include <string>

class ConfigLoader {
public:
	int verMajor = 3;
	int verMinor = 3;
	int profile = GLFW_OPENGL_CORE_PROFILE;
	int forwardCompat = GL_TRUE;
	int verShader = 330;

	int wHeight = 800;
	int wWidth = 600;
	std::string wScreenMode = "BORDERLESS";
	std::string wName = "ALF";

	ConfigLoader(const char* filename);
};