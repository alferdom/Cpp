#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <vector>
#include <string>


class Model
{
private:
	const std::string modelPath;
	std::vector<glm::vec3> & vertices;
	std::vector<glm::vec2> & uvs;
	std::vector<glm::vec3> & normals;

public:
	~Model();
	Model(const std::string& objPath);
	bool Load();
	bool LoadWithMTL();

	void Draw(const Shader& shader) const;
};