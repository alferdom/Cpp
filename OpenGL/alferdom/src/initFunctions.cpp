#pragma	once

#include "initFunctions.h"

GLFWwindow* initGLFW(const ConfigLoader& configLoader) {
    GLFWwindow* window;

    /* Initialize the glfw library */
    if (!glfwInit())
        exit(-1);

    /* Specification of OpenGL */
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configLoader.verMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configLoader.verMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, configLoader.profile);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, configLoader.forwardCompat);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    if (configLoader.wScreenMode == "BORDERLESS")
        window = glfwCreateWindow(mode->width, mode->height, configLoader.wName.c_str(), NULL, NULL);
    else if (configLoader.wScreenMode == "FULLSCREEN")
        window = glfwCreateWindow(mode->width, mode->height, configLoader.wName.c_str(), glfwGetPrimaryMonitor(), NULL);
    else
        window = glfwCreateWindow(configLoader.wWidth, configLoader.wHeight, configLoader.wName.c_str(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(-2);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    return window;
}

void initImGui(GLFWwindow* window, const char* shaderVersion) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(shaderVersion);
}


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}