#pragma once

#include "alferdom.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>

#include "glm/gtx/transform.hpp"

ConfigLoader configLoader("./Config/config.ini");
// moving camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// static cameras
Camera staticCamera1(glm::vec3(-2.0f, 6.0f, 3.0f), glm::vec3(3.0f, -6.0f, -4.0f));
Camera staticCamera2(glm::vec3(8.0f, 4.5f, -3.0f), glm::vec3(-6.0f, -2.0f, -2.0f));

glm::mat4 View = camera.getViewMatrix();

float lastX;
float lastY;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool useDynamicCamera = true;
bool batCameraView = false;
bool bannerShow = false;
bool bannerCreate = false;
bool litFire = false;
bool flyBat = false;
bool useFlashLight = false;

bool isVectorNull(const glm::vec3& vect) {

    return !vect.x && !vect.y && !vect.z;
}


glm::mat4 alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

    glm::vec3 z = -glm::normalize(front);

    if (isVectorNull(z))
        z = glm::vec3(0.0, 0.0, 1.0);

    glm::vec3 x = glm::normalize(glm::cross(up, z));

    if (isVectorNull(x))
        x = glm::vec3(1.0, 0.0, 0.0);

    glm::vec3 y = glm::cross(z, x);
    glm::mat4 matrix = glm::mat4(
        x.x, x.y, x.z, 0.0,
        y.x, y.y, y.z, 0.0,
        z.x, z.y, z.z, 0.0,
        position.x, position.y, position.z, 1.0
    );
    return matrix;
}

int main(void)
{
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

    std::vector<Shader> vecShadersToLight;
 
    Renderer renderer;

    DirectionLight skylight(glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(2.0f, 0.0f, 0.0f));
    PointLight pointLight1(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 8.0f, -3.0f), 1.0f, 0.09f, 0.032f, 0);
    PointLight pointLight2(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.7f, 0.2f, 2.0f), 1.0f, 0.09f, 0.032f, 1);
    FlashLight flashlight(camera.Position, camera.Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(7.5f)), glm::cos(glm::radians(10.0f)));

    Shader shaderSkybox("./res/shaders/Skybox.shader");
    Skybox skyboxDay("./res/skybox/day");
    Skybox skyboxNight("./res/skybox/night");
    shaderSkybox.Bind();
    shaderSkybox.SetUniform1i("cubeSkybox", 0);
    shaderSkybox.Unbind();

    Shader shaderBat("./res/shaders/Bat.shader");
    //vecShadersToLight.push_back(shaderBat);
    DynamicModel Bat("./res/models/bat/Bat_0.obj", true, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, static_cast<float>(glfwGetTime()));

    Shader shaderModelDefault("./res/shaders/DefaultModel.shader");
    vecShadersToLight.push_back(shaderModelDefault);
    Model fireplace("./res/models/fireplace/fireplace.obj", true, glm::vec3(0.0f, -0.5f, -5.0f));

    Texture fireTexture;
    fireTexture.id = fireTexture.TextureFromFile("fireEffect.jpg", "./res/textures");
    fireTexture.type = "texture_diffuse";
    std::vector<Texture> vecText = { fireTexture };

    Shader shaderFire("./res/shaders/Fire.shader");
    AnimatedTexture fireText(fireTexture, new Mesh(fireVertexData, fireIndices, vecText, nullptr), fireplace.getPosition() + glm::vec3(0.0f, 1.0f, 0.0f), static_cast<float>(glfwGetTime()));

    stbi_set_flip_vertically_on_load(true);
    Texture bannerTexture;
    bannerTexture.id = bannerTexture.TextureFromFile("Batman-Logo-banner-drawing.jpeg", "./res/textures");
    bannerTexture.type = "texture_diffuse";
    stbi_set_flip_vertically_on_load(false);

    Shader shaderBanner("./res/shaders/Banner.shader");
    AnimatedTexture* banner = nullptr;

    /*for (size_t i = 0; i < batAnim.numFrames; i++)
    {
        vecBats.emplace_back("./res/models/bat/Bat_" + std::to_string(i) + ".obj", true, glm::vec3(0.0f, 0.0f, 0.0f));
    }*/

   //Shader shaderBatmobile("./res/shaders/Batmobile.shader");
   // Model batmobile("./res/models/batmobile/batmobile8.obj", false, glm::vec3(-10.0f, 0.0f, 0.0f));

    CatmullRow catmullRow;

    glm::mat4 Projection;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Clearing framebuffers
        renderer.Clear();

        // time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //unsigned e = (unsigned)currentFrame % batAnim.frameMs;
        //batAnim.setFrame((unsigned)currentFrame, e);

        // input
        processInput(window);

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

        Projection = glm::perspective(glm::radians(camera.getZoom()), (float)configLoader.wWidth / (float)configLoader.wHeight, 0.1f, 100.0f);

       // setting Bat position based on Catmull-Row curve
       catmullRow.paramT = Bat.getSpeed() * (currentFrame - Bat.startTime);
       Bat.setPosition(glm::vec3(2.0f, 0.0f, 0.0f) + catmullRow.getResultPosition());
       Bat.setDirection(glm::normalize(catmullRow.getResultDirection()));     
       shaderBat.Bind();
       shaderBat.SetUniform1f("time", currentFrame);
       shaderBat.Unbind();

       glm::mat4 Model = alignObject(Bat.getPosition(), Bat.getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
       Model = glm::scale(Model, glm::vec3(0.02f, 0.02f, 0.02f));

       if(batCameraView){
           glm::vec3 cameraPosition = Bat.getPosition() - 0.5f * Bat.getDirection();
           glm::vec3 cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
       
           glm::vec3 cameraViewDirection = Bat.getPosition();
           glm::mat4 cameraRot = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), cameraUpVector);
           cameraPosition = glm::vec3(cameraRot * glm::vec4(cameraPosition, 0.0f));
           //cameraViewDirection = glm::vec3(cameraRot * glm::vec4(cameraViewDirection, 0.0f));
           //glm::vec3 cameraCenter = cameraPosition + cameraViewDirection;
       
           View = glm::lookAt(
               cameraPosition,
               cameraViewDirection,
               cameraUpVector
           );
       }

        //shaderBat.Bind();
        //shaderBat.SetUniform3f("viewPos", camera.Position);

       // Drawing lights
       for (auto& it : vecShadersToLight)
        {   
            it.Bind();
            it.SetUniform3f("viewPos", camera.Position);
            it.SetUniform1b("useFlashLight", useFlashLight);
            skylight.Draw(it);
            pointLight1.Draw(it);
            pointLight2.Draw(it);
            if (useFlashLight) {
                flashlight.direction = glm::vec4(camera.Front, 0.0f);
                flashlight.position = camera.Position;
                flashlight.Draw(it);
            }
            it.Unbind();
        }

        // Drawing calls
        unsigned id = 1;
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
       
        glStencilFunc(GL_ALWAYS, id, -1);
        Bat.DrawDynamic(shaderBat, Model, View, Projection, camera.Position);

        id++;

       // shaderModelDefault.Bind();
       // shaderModelDefault.SetUniform3f("viewPos", camera.Position);
       // shaderModelDefault.SetUniform1b("useFlashLight", useFlashLight);
       // skylight.Draw(shaderModelDefault);
       // pointLight1.Draw(shaderModelDefault);
       // pointLight2.Draw(shaderModelDefault);
       // if (useFlashLight) {
       //     flashlight.direction = glm::vec4(camera.Front, 0.0f);
       //     flashlight.position = camera.Position;
       //     flashlight.Draw(shaderModelDefault);
       // }

       glStencilFunc(GL_ALWAYS, id, -1);
       fireplace.Draw(shaderModelDefault, 0.2f, View, Projection, camera.Position, camera.Front);

       // batmobile.Draw(shaderBatmobile, 0.4, View, Projection, camera.Position);

       /* if (fmod(currentFrame, 20.0) < 10.0)
           skyboxNight.Draw(shaderSkybox, View, Projection);
        else*/
           skyboxDay.Draw(shaderSkybox, View, Projection);

        if (litFire) {
             glEnable(GL_BLEND);
             glBlendFunc(GL_ONE, GL_ONE);
             fireText.Draw(shaderFire, 1.4f, View, Projection, currentFrame);
             glDisable(GL_BLEND);
         }

        if (bannerCreate && banner == nullptr && useDynamicCamera) {
            vecText[0] = bannerTexture;
            banner = new AnimatedTexture(bannerTexture, new Mesh(fireVertexData, fireIndices, vecText, nullptr), glm::vec3(0.0, 0.0, 0.0), static_cast<float>(glfwGetTime()));
        }
        else if(!bannerCreate && banner != nullptr){
            delete banner;
            banner = nullptr;
            bannerCreate = false;
        }

        if (banner != nullptr) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);
            (*banner).position = camera.Position + 2.0f*camera.Front;
            (*banner).Draw(shaderBanner,0.8f, View, Projection, currentFrame);
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

   // if (banner != nullptr)
   //     delete banner;

    //Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (useDynamicCamera) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        View = camera.getViewMatrix();
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        batCameraView = !batCameraView;
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        View = camera.getViewMatrix();
        useDynamicCamera = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        View = staticCamera1.getViewMatrix();
        useDynamicCamera = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
        View = staticCamera2.getViewMatrix();
        useDynamicCamera = false;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        useFlashLight = !useFlashLight;

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        bannerCreate = !bannerCreate;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    { 
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        unsigned char objID = 0;
        
        glReadPixels(mouseX, configLoader.wHeight - 1 - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &objID);

        if (objID == 1) {
            flyBat = !flyBat;
        }

        if (objID == 2) {
            litFire = !litFire;
        }
    }
}


// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}