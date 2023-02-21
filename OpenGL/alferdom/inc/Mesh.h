#pragma once

#include "MeshStructs.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"

#include <string>
#include <vector>


class Mesh {
public:
    // mesh Data
    const std::vector<Vertex>      vecVertices;
    const std::vector<unsigned int>  vecIndices;
    const std::vector<Texture> vecTextures;
    const Material* meshMaterial = nullptr;

    VertexArray VAO;
    VertexBuffer VBO;
    ElementBuffer EBO;

    // mesh constructors
    Mesh() = default;
    Mesh(const std::vector<Vertex> & vertices, const std::vector<unsigned int> & indices, const std::vector<Texture> & textures, const Material* mat);
    Mesh(const std::vector<Vertex> & vertices, const std::vector<unsigned int> & indices, const Material* mat);

    ~Mesh();

    // render mesh
    void Draw(Shader& shader) const;

private:
    // initializes all the buffer objects/arrays
    void setupMesh();
};