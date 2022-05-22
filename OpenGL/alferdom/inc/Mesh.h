#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "ElementBuffer.h"


#include <vector>
#include <string>

//#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:

    // mesh Data
    const std::vector<Vertex>       vecVertices;
    const std::vector<unsigned int> vecIndices;
    const std::vector<Texture>      vecTextures;

    VertexArray VAO;
    VertexBuffer VBO;
    ElementBuffer EBO;

    // constructor
    Mesh(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices, std::vector<Texture> & textures);

    // render the mesh
    void Draw(Shader& shader);

private:
    // initializes all the buffer objects/arrays
    void setupMesh();
};