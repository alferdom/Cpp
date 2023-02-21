#pragma once

#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"

#include <string>
#include <vector>


class Skybox
{
public:
	Skybox(const std::string& skyboxDIR);
	~Skybox();

	void Draw(Shader& shader,const glm::mat4& view, const glm::mat4& projection) const;
	//unsigned int Load(const std::string& path, std::vector<std::string>& faces) const;
private:
	unsigned int Load(const std::string& path) const;
	unsigned int textureID;

	VertexArray VAO;
	VertexBuffer VBO;
	ElementBuffer EBO;

	std::vector<std::string> faces{
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
	};
};
