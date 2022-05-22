#pragma once

#include "Shader.h"
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <sstream>


Shader::Shader(const std::string& shaderPath) : ShaderPath(shaderPath), RendererID(0)
{
	ShaderSource src = ParseShader();
	RendererID = CreateShader(src.VertexSource, src.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(RendererID);
}

ShaderSource Shader::ParseShader()
{
	std::ifstream shaderFile(ShaderPath);

	enum class ShaderType : int {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	
	while (getline(shaderFile, line)) {
		if (line.find("#shader") != line.npos) {
			if (line.find("vertex") != line.npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != line.npos)
				type = ShaderType::FRAGMENT;	
		}
		else {
			ss[(int)type] << line << std::endl;
		}
	}

	return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{	
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		GLchar* msg = new GLchar[length];
		glGetShaderInfoLog(id, length, &length, msg);
		std::cout << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader" << std::endl;
		std::cout << msg << std::endl;
		glDeleteShader(id);
		delete [] msg;
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vertShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

void Shader::Bind() const
{
	glUseProgram(RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (mapUniformLocations.find(name) != mapUniformLocations.end())
		return mapUniformLocations[name];

	int location = glGetUniformLocation(RendererID, name.c_str());
	if (location != -1)
		std::cout << "Error: get Uniform " << name << "not found!" << std::endl;

	mapUniformLocations[name] = location;
	return location;
}

void Shader::SetUniform4f(const std::string& name, glm::vec4& data)
{
	glUniform4f(GetUniformLocation(name), data.x, data.y, data.z, data.w);
}

void Shader::SetUniform1i(const std::string& name, unsigned int data) 
{
	glUniform1i(GetUniformLocation(name), data);
}