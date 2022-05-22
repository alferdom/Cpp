#pragma once

#include "alferdom.h"

GLFWwindow* initGLFW(const ConfigLoader& configLoader);

void initImGui(GLFWwindow* window, const char* shaderVersion);

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);