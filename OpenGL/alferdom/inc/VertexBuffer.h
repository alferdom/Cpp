#pragma once

class VertexBuffer
{
private:
	unsigned int RendererID;

public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};