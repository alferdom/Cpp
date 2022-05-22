#pragma once

#include "Mesh.h"

#include <GL/glew.h>

Mesh::Mesh(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices, std::vector<Texture> & textures) : vecVertices(vertices), vecIndices(indices), vecTextures(textures)
{
	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < vecTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = vecTextures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // set the sampler to the correct texture unit
        shader.SetUniform1i(name + number, i);

        // bind the texture
        glBindTexture(GL_TEXTURE_2D, vecTextures[i].id);
    }
}

void Mesh::setupMesh()
{
    //setup buffers
    VBO = VertexBuffer(&vecVertices[0], vecVertices.size() * sizeof(Vertex));
    EBO = ElementBuffer(&vecIndices[0], vecIndices.size() * sizeof(Vertex));

    // layouts for Mesh attributes
    VertexBufferLayout layout;
    layout.Push<Vertex>(3);
    layout.Push<Vertex>(3);
    layout.Push<Vertex>(2);
    layout.Push<Vertex>(3);
    layout.Push<Vertex>(4);

    VAO.AddBuffer(VBO, layout);

    VAO.Unbind();
}
