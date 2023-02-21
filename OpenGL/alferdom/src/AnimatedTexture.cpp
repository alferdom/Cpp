#include "AnimatedTexture.h"
#include "glm/gtx/transform.hpp"

AnimatedTexture::AnimatedTexture(const Texture& newText, const Mesh* newMesh, const glm::vec3& newPosition, const float newStartTime)
	: texture(newText), mesh(newMesh), position(newPosition), startTime(newStartTime)
{}

AnimatedTexture::~AnimatedTexture()
{
	delete mesh;
}

void AnimatedTexture::Draw(Shader & shader, const float size, const glm::mat4& View, const glm::mat4& Projection, const float currentime) const
{
	shader.Bind();

	glm::mat4 billboardMat = glm::transpose(
		glm::mat4(
		View[0],
		View[1],
		View[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	));

	glm::mat4 Model = glm::translate(glm::mat4(1.0f), position);
	Model = glm::scale(Model, glm::vec3(size, size, size));
	Model = Model * billboardMat; // make billboard to face the camera
	shader.SetUniformMatrix4fv("Model", Model);
	shader.SetUniformMatrix4fv("View", View);
	shader.SetUniformMatrix4fv("Projection", Projection);
	shader.SetUniform1f("time", currentime - startTime);

	(*mesh).Draw(shader);

	shader.Unbind();
}
