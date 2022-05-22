#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ConfigLoader.h"
#include "InitFunctions.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Model.h"