#pragma once

#include "ConfigLoader.h"

#include <iostream>
#include <fstream>

enum class Type : int
{
	OPENGL,
	SHADER,
	WINDOW,
};

ConfigLoader::ConfigLoader(const char* filename)
{
	std::string line, value;
	std::ifstream myfile(filename);
	Type curType = Type::OPENGL;


	if (myfile.good()) {
		while (std::getline(myfile, line)) {

			if (line == "OPENGL") {
				curType = Type::OPENGL;
			}
			else if (line == "SHADER") {
				curType = Type::SHADER;
			}
			else if (line == "WINDOW") {
				curType = Type::WINDOW;
			}
			else {
				value = line.substr(line.find("=") + 1);

				switch (curType)
				{
				case Type::OPENGL:
					if (line.find("VERSION_MAJOR") != line.npos)
						verMajor = value[0] - '0';
					else if (line.find("VERSION_MINOR") != line.npos)
						verMinor = value[0] - '0';
					else if (line.find("PROFILE") != line.npos) {
						if (value == "GLFW_OPENGL_CORE_PROFILE")
							profile = GLFW_OPENGL_CORE_PROFILE;
						else if (value == "GLFW_OPENGL_COMPAT_PROFILE")
							profile = GLFW_OPENGL_COMPAT_PROFILE;
						else if (value == "GLFW_OPENGL_ANY_PROFILE")
							profile = GLFW_OPENGL_ANY_PROFILE;
					}
					else if (line.find("FORWARD_COMPAT") != line.npos) {
						if (value == "TRUE")
							forwardCompat = GL_TRUE;
						else
							forwardCompat = GL_FALSE;
					}
					break;

				case Type::SHADER:
					if (line.find("VERSION") != line.npos) {
						verShader = std::stoi(value);
					}
					break;

				case Type::WINDOW:

					if (line.find("HEIGHT") != line.npos) {
						wHeight = std::stoi(value);
					}
					else if (line.find("WIDTH") != line.npos) {
						wWidth = std::stoi(value);
					}
					else if (line.find("SCREEN_MODE") != line.npos) {
						wScreenMode = value;
					}
					else if (line.find("NAME") != line.npos) {
						wName = value;
					}
					break;
				}
			}
		}
	}
	myfile.close();
}
