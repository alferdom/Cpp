#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "Shader.h"
#include "ElementBuffer.h"

class Renderer
{
private:
public:
	Renderer() {};
	~Renderer() {};

	void Clear() const;
	void Draw(const VertexArray& VAO, const ElementBuffer& EBO, const Shader& shader) const;
};
 