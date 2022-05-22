#pragma once

#include <GL/glew.h>
#include "Mesh.h"

#include <vector>
#include <iostream>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned int normalized;
	unsigned int offset;

	static unsigned int getSizeOfType(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT:
				return sizeof(GL_FLOAT);
			case GL_UNSIGNED_INT:
				return sizeof(GL_UNSIGNED_INT);
			case GL_UNSIGNED_BYTE:
				return sizeof(GL_UNSIGNED_BYTE);
			case 1:
				return sizeof(Vertex);
		}
		std::cout << "Wrong size of type : " << type << std::endl;

		return 0;
	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> vecElements;
	unsigned int Stride;

public:
	VertexBufferLayout() : Stride(0) {};
	~VertexBufferLayout() {};

	template<typename T> void Push(unsigned int count);
	template<> void Push<float>(unsigned int count) {
		vecElements.push_back({GL_FLOAT, count, GL_FALSE, count * VertexBufferElement::getSizeOfType(GL_FLOAT)});
		Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	};

	template<> void Push<unsigned int>(unsigned int count) {
		vecElements.push_back({GL_UNSIGNED_INT, count, GL_FALSE, count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT)});
		Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	};

	template<> void Push<unsigned char>(unsigned int count) {
		vecElements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE, count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE)});
		Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	};

	template<> void Push<Vertex>(unsigned int count) {
		vecElements.push_back({ GL_FLOAT, count, GL_FALSE, count * VertexBufferElement::getSizeOfType(GL_FLOAT) });
		Stride = VertexBufferElement::getSizeOfType(1);
	};

	inline const std::vector<VertexBufferElement>& getElements() const {
		return vecElements;
	}

	inline unsigned int getStride() const {
		return Stride;
	};
};