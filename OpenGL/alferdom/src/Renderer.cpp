#pragma once

#include "Renderer.h"

Renderer::Renderer()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT_AND_BACK);
	//glCullFace(GL_BACK);
}

void Renderer::Clear() const
{
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //| GL_STENCIL_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& VAO, const ElementBuffer& EBO, const Shader& shader) const
{
	shader.Bind();
	VAO.Bind();
	EBO.Bind();

	glDrawElements(GL_TRIANGLES, EBO.getCount(), GL_UNSIGNED_INT, NULL);
}
