#pragma once

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int RendererID;
	const std::string ShaderPath;
	std::unordered_map<std::string, int> mapUniformLocations;

	ShaderSource ParseShader();
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	unsigned int GetUniformLocation(const std::string& name);
public:
	Shader(const std::string& shaderPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, glm::vec4& data);
	void SetUniform1i(const std::string& name, unsigned int data);
};