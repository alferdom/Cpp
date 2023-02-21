#include "Light.h"

Light::Light(const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular) : ambient(nAmbient), diffuse(nDiffuse), specular(nSpecular)
{}

DirectionLight::DirectionLight(const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular, const glm::vec3 nDirection) : Light(nAmbient, nDiffuse, nSpecular), direction(glm::vec4(nDirection, 0.0f))
{}

void DirectionLight::Draw(Shader & shader) const
{
	shader.SetUniform3f("dirLight.direction", glm::vec3(direction));
	shader.SetUniform3f("dirLight.ambient", ambient);
	shader.SetUniform3f("dirLight.diffuse", diffuse);
	shader.SetUniform3f("dirLight.specular", specular);
}

PointLight::PointLight(const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular, const glm::vec3 nPosition,
	const float nConstant, const float nLinear, const float nQuadratic, unsigned short nNumber)
	: Light(nAmbient, nDiffuse, nSpecular), position(nPosition), constant(nConstant), linear(nLinear), quadratic(nQuadratic), number(std::to_string(nNumber))
{}

void PointLight::Draw(Shader & shader) const
{
	shader.SetUniform3f("pointLights[" + number + "].position", position);
	shader.SetUniform3f("pointLights[" + number + "].ambient", ambient);
	shader.SetUniform3f("pointLights[" + number + "].diffuse", diffuse);
	shader.SetUniform3f("pointLights[" + number + "].specular", specular);
	shader.SetUniform1f("pointLights[" + number + "].constant", constant);
	shader.SetUniform1f("pointLights[" + number + "].linear", linear);
	shader.SetUniform1f("pointLights[" + number + "].quadratic", quadratic);
}

FlashLight::FlashLight(const glm::vec3 nPosition, const glm::vec3 nDirection, const glm::vec3 nAmbient, const glm::vec3 nDiffuse, const glm::vec3 nSpecular,
	const float nConstant, const float nLinear, const float nQuadratic, const float nCutOff, const float nOuterCutOff)
	: PointLight(nAmbient, nDiffuse, nSpecular, nPosition, nConstant, nLinear, nQuadratic, 0), cutOff(nCutOff), outerCutOff(nOuterCutOff), direction(glm::vec4(nDirection, 0.0f))
{}

void FlashLight::Draw(Shader& shader) const
{
	shader.SetUniform3f("flashLight.position", position);
	shader.SetUniform3f("flashLight.direction", glm::vec3(direction));
	shader.SetUniform3f("flashLight.ambient", ambient);
	shader.SetUniform3f("flashLight.diffuse", diffuse);
	shader.SetUniform3f("flashLight.specular", specular);
	shader.SetUniform1f("flashLight.constant", constant);
	shader.SetUniform1f("flashLight.linear", linear);
	shader.SetUniform1f("flashLight.quadratic", quadratic);
	shader.SetUniform1f("flashLight.cutOff", cutOff);
	shader.SetUniform1f("flashLight.outerCutOff", outerCutOff);
}
