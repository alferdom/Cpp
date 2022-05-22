#pragma once

#include "Renderer.h"

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& VAO, const ElementBuffer& EBO, const Shader& shader) const
{
	shader.Bind();
	VAO.Bind();
	EBO.Bind();

	glDrawElements(GL_TRIANGLES, EBO.getCount(), GL_UNSIGNED_INT, NULL);
}
