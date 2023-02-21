#pragma once

#include "glm/glm.hpp"
#include <string>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;

    Vertex(): Position(glm::vec3(0.0f, 0.0f, 0.0f)), Normal(glm::vec3(0.0f, 0.0f, 0.0f)), TexCoords(glm::vec3(0.0f, 0.0f, 0.0f)) {}

    Vertex(glm::vec3 & position, glm::vec3 & normal, glm::vec2 & texCoords) 
        : Position(position), Normal(normal), TexCoords(texCoords) {};

    Vertex(const glm::vec3& position, const glm::vec3& normal,const glm::vec2& texCoords)
        : Position(position), Normal(normal), TexCoords(texCoords) {}

    bool operator< (const Vertex rhs) const
    {
        return memcmp((void*)this, (void*)&rhs, sizeof(Vertex)) > 0;
    } 
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
