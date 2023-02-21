#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "data.h"
#include "InitFunctions.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Skybox.h"
#include "Light.h"
#include "CatmullRow.h"
#include "AnimatedTexture.h"

int main(void);
void processInput(GLFWwindow* window);