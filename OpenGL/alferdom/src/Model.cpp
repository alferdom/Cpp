#pragma once

#include "Model.h"

#include <iostream>
#include <fstream>


Model::Model(const std::string & objPath) : modelPath(objPath) {}

bool Model::Load()
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;



	std::cout << "Loading OBJ file " << modelPath << std::endl;
	return false;
}
