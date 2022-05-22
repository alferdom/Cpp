#pragma once

#include "alferdom.h"

#include <iostream>
#include <string>


int main(void)
{
    ConfigLoader configLoader("./Config/config.ini");

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    bool show_demo_window = true;
    bool show_another_window = false;

    GLFWwindow* window = initGLFW(configLoader);

    /* Initialize the glew library */
    if (glewInit() != GLEW_OK)
        return -2;

    const std::string versionShader = "#version " + std::to_string(configLoader.verShader);
    initImGui(window, "#version 330");

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    VertexArray VAO;
    //VertexBuffer VBO({1, 5,6 ,65 }, 2 * 2 * sizeof(GLint));
    //VertexBufferLayout layout;
    //VAO.AddBuffer(VBO, layout);
    //ElementBuffer EBO();

    Shader shader("./res/shaders/Basic.shader");
    shader.Bind();

    VAO.Unbind();
    //VBO.Unbind();
    //EBO.Unbind();
    shader.Unbind();
   
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

       {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        renderer.Clear();

        //renderer.Draw();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}