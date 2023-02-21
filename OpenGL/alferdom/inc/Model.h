#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>


class Model
{
protected:
	const std::string modelPath;
	const std::string modelDirectory;

	std::vector<Mesh*> vecMeshes;
	std::vector<Texture*> vecTextures;
	std::vector<Material*> vecMaterials;
	std::unordered_map<std::string, unsigned int> mapMaterials;
	glm::vec3 position;

	bool LoadMTL();
	bool OwnLoad();
	void LoadWithTextures();
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture>  loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	

public:
	Model(const std::string& objPath, const bool LoadTextures, glm::vec3 newPosition);
	~Model();

	void Draw(Shader& shader, const float size, glm::mat4& View, const glm::mat4& Projection, glm::vec3& cameraPos, glm::vec3& cameraFront) const;

	unsigned int TextureFromFile(const char* path, const std::string& directory);

	glm::vec3 getPosition() const;
};


struct ModelAnimation {
	const unsigned short numFrames;
	unsigned short currFrame;
	const unsigned int short frameMs = 150;
	float relativeTime;

	ModelAnimation(const unsigned short cntFrames);
	void setFrame(unsigned int time, const int relFrameTime);
};

class DynamicModel : public Model
{
public:
	DynamicModel(const std::string& objPath, const bool LoadTextures, glm::vec3 newPosition, glm::vec3 newDirection, float newSpeed, float newStartTime);
	//~DynamicModel();

	void DrawDynamic(Shader& shader, const glm::mat4& Model, const glm::mat4& View, const glm::mat4& Projection, const glm::vec3 cameraPos) const;

	glm::vec3 getDirection() const;
	void setDirection(glm::vec3 newDir);

	glm::vec3 getPosition() const;
	void setPosition(const glm::vec3 newPos);

	float getSpeed() const;
	void setSpeed(const float newSpeed);

	const float startTime;

private:
	float speed;
	glm::vec3 direction;
};