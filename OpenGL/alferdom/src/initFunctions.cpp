#pragma	once

#include <GL/glew.h>
#include "initFunctions.h"

GLFWwindow* initGLFW(const ConfigLoader& configLoader) {
    GLFWwindow* window;

    /* Initialize the glfw library */
    if (!glfwInit())
        exit(-1);

    /* Specification of OpenGL */
    //glfwWindowHint(GLFW_SAMPLES, 4); /// 4xMSAA
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

    // Make the window's context current adn set controls callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // enable sticky keys checking
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, configLoader.wWidth / 2, configLoader.wHeight / 2);

    //glfwPollEvents();

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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
