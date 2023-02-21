#pragma once
#include "glm/glm.hpp"
#include "Shader.h"

class Light
{
public:
	Light(const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular);
	~Light() = default;

	virtual void Draw(Shader& shader) const = 0;

protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};


class DirectionLight : public Light
{
public:
	DirectionLight(const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular, const glm::vec3 nDirection);
	~DirectionLight() = default;

	void Draw(Shader& shader) const override;

private:
	const glm::vec4 direction;
};


class PointLight : public Light
{
public:
	PointLight(const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular, const glm::vec3 nPosition,
		const float nConstant, const float nLinear, const float nQuadratic, unsigned short nNumber);
	~PointLight() = default;

	void Draw(Shader& shader) const override;

	glm::vec3 position;

private:
	std::string number;

protected:
	
	const float constant;
	const float linear;
	const float quadratic;
};


class FlashLight : public PointLight
{
public:
	FlashLight(const glm::vec3 nPosition, const glm::vec3 nDirection, const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular,
		const float nConstant, const float nLinear, const float nQuadratic, const float nCutOff, const float nOuterCutOff);
	~FlashLight() = default;

	void Draw(Shader& shader) const override;
	glm::vec4 direction;
private:
	const float cutOff;
	const float outerCutOff;
};

