#pragma once

#include "Mesh.h"

#include <GL/glew.h>

Mesh::Mesh(const std::vector<Vertex> & vertices, const std::vector<unsigned int> & indices,const std::vector<Texture> & textures, const Material* mat) : vecVertices(vertices), vecIndices(indices), vecTextures(textures), meshMaterial(mat),
VAO(VertexArray()), VBO(VertexBuffer(&vecVertices[0], vecVertices.size() * sizeof(Vertex))), EBO(ElementBuffer(&vecIndices[0], vecIndices.size()))
{
	setupMesh();
}
 
Mesh::Mesh(const std::vector<Vertex> & vertices,const std::vector<unsigned int> & indices,const Material* mat) : vecVertices(vertices), vecIndices(indices), vecTextures(std::vector<Texture>()), meshMaterial(mat),
VAO(VertexArray()), VBO(VertexBuffer(&vecVertices[0], vecVertices.size() * sizeof(Vertex))), EBO(ElementBuffer(&vecIndices[0], vecIndices.size()))
{
    setupMesh();
}

Mesh::~Mesh()
{
    meshMaterial = nullptr;
    VAO.~VertexArray();
    VBO.~VertexBuffer();
    EBO.~ElementBuffer();
}

void Mesh::Draw(Shader& shader) const
{
    //shader.Bind();
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    // Texture drawing
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
    glActiveTexture(GL_TEXTURE0);

    // Material drawing
    if (meshMaterial) {
        //shader.SetUniform3f("material.ambient", meshMaterial->ambient);
       // shader.SetUniform3f("material.diffuse", meshMaterial->diffuse);
        shader.SetUniform3f("material.specular", meshMaterial->specular);
        shader.SetUniform1f("material.shininess", meshMaterial->shininess);
    }
    // Buffer binding and Elements draw
    VAO.Bind();

    glDrawElements(GL_TRIANGLES, vecIndices.size(), GL_UNSIGNED_INT, 0);
    
    VAO.Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::setupMesh()
{
    // layouts for Mesh attributes
    VertexBufferLayout layout;
    layout.Push<Vertex>(3, GL_FALSE);
    layout.Push<Vertex>(3, GL_TRUE);
    layout.Push<Vertex>(2, GL_FALSE);

    VAO.AddBuffer(VBO, layout);

    VAO.Unbind();
}
