#pragma once

#include "ElementBuffer.h"

#include <GL/glew.h>

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count) : Count(count)
{
	glGenBuffers(1, &RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &RendererID);

}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);

}

void ElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
