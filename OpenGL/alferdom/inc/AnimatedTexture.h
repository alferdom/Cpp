#pragma once

#include "Mesh.h"
#include "MeshStructs.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"

class AnimatedTexture
{
public:
	AnimatedTexture(const Texture & newText,const Mesh* newMesh,const glm::vec3 & newPosition, const float newStartTime);
	~AnimatedTexture();

	void Draw(Shader& shader, const float size, const glm::mat4& View, const glm::mat4& Projection, const float currentime)const;

	glm::vec3 position;
	const float startTime;

private:
	const Texture texture;
	const Mesh* mesh;
};