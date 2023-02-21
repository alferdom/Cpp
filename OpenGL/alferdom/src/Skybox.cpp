#include "Skybox.h"

#include <GL/glew.h>
#include <stb_image.h>
#include <iostream>

float cubeVertices[24] = {
 -1.0f,  1.0f, -1.0f,
 -1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f,  1.0f, -1.0f,

 -1.0f, -1.0f,  1.0f,
 -1.0f,  1.0f,  1.0f,
 1.0f, -1.0f,  1.0f,
 1.0f,  1.0f,  1.0f
};


unsigned int cubeIndices[36] = {
     0, 1, 2,
     2, 3, 0,
     4, 1, 0,
     0, 5, 4,
     2, 6, 7,
     7, 3, 2,
     4, 5, 7,
     7, 6, 4,
     0, 3, 7,
     7, 5, 0,
     1, 4, 2,
     2, 4, 6
};

Skybox::Skybox(const std::string& skyboxDIR) : VAO(VertexArray()), VBO(VertexBuffer(&cubeVertices[0], sizeof(cubeVertices))), EBO(ElementBuffer(&cubeIndices[0], sizeof(cubeIndices) / sizeof(cubeIndices[0])))
{
    VertexBufferLayout layout;
    layout.Push<float>(3);

    VAO.AddBuffer(VBO, layout); 

    textureID = Load(skyboxDIR);
}

Skybox::~Skybox()
{
    VBO.~VertexBuffer();
    EBO.~ElementBuffer();
    VAO.~VertexArray();  
}

void Skybox::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const
{   
    glDepthFunc(GL_LEQUAL);
    shader.Bind();

    shader.SetUniformMatrix4fv("view", glm::mat4(glm::mat3(view)));  // remove possible translation
    shader.SetUniformMatrix4fv("projection", projection);

    VAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawElements(GL_TRIANGLES, EBO.getCount(), GL_UNSIGNED_INT, 0);

    VAO.Unbind();
    shader.Unbind();
    glDepthFunc(GL_LESS);
}

unsigned int Skybox::Load(const std::string& skyboxDIR) const
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load((skyboxDIR + "/" + faces[i]).c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureID;
}
