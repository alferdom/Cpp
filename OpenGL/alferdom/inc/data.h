#pragma once

#include "MeshStructs.h"
#include <vector>

const std::vector<Vertex> fireVertexData = {
	Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f))
};

const std::vector<unsigned int> fireIndices = {
	0, 1, 2, 
	1, 3, 2
};

