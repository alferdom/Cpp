#pragma once 

#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& VBO, const VertexBufferLayout& layout)
{
	Bind();
	VBO.Bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (size_t i = 0; i < elements.size(); i++)
	{
		const auto& elem = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elem.count, elem.type, elem.normalized, layout.getStride(), (const void*) offset);
		offset += elem.offset;
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
	
