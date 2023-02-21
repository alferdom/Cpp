#pragma once

#include "Model.h"

#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <map>


void parseFloats(Material* mat, const std::string& line, unsigned short int& colorType) {
    switch (colorType)
    {
    case 0:
        mat->ambient.r = std::stof(line);
        mat->ambient.g = std::stof(line.substr(line.find_first_of(' ')));
        mat->ambient.b = std::stof(line.substr(line.find_last_of(' ')));
        break;

    case 1:
        mat->diffuse.r = std::stof(line);
        mat->diffuse.g = std::stof(line.substr(line.find_first_of(' ')));
        mat->diffuse.b = std::stof(line.substr(line.find_last_of(' ')));
        break;

    case 2:
        mat->specular.r = std::stof(line);
        mat->specular.g = std::stof(line.substr(line.find_first_of(' ')));
        mat->specular.b = std::stof(line.substr(line.find_last_of(' ')));
        break;
    }

    colorType++;
}

void parseVec3Floats(const std::string& line, glm::vec3 & vec3) {
    vec3.x = std::stof(line);
    vec3.y = std::stof(line.substr(line.find_first_of(' ')));
    vec3.z = std::stof(line.substr(line.find_last_of(' ')));
}

void parseVec2Floats(const std::string& line, glm::vec2& vec2) {
    vec2.x = std::stof(line);
    vec2.y = std::stof(line.substr(line.find_first_of(' ')));
}

void parseFace(const std::string& curline, std::vector<unsigned int> & vertexIndices, std::vector<unsigned int> & normalIndices, std::vector<unsigned int> & uvIndices) {
    auto line = curline;
    for (unsigned short i = 0; i < 3; i++)
    {
        vertexIndices.push_back(std::stoul(line));

        line = line.substr(line.find_first_of('/') + 1);

        if (line[0] != '/') {
            uvIndices.push_back(std::stoul(line));           
        }
        line = line.substr(line.find_first_of('/') + 1);

        normalIndices.push_back(std::stoul(line));
        line = line.substr(line.find_first_of(' ') + 1);
    }
}

template <typename T, typename T2>
void clearVectors(std::vector<T> & vec1, std::vector<T> & vec2, std::vector<T2> & vec3) {
    vec1.clear();
    vec2.clear();
    vec3.clear();
}

bool findSimilarVertex(
    const Vertex & vertex,
    const std::map<Vertex, unsigned int>& mapVertexOutIndex,
    unsigned int & result
) {
    auto it = mapVertexOutIndex.find(vertex);
    if (it != mapVertexOutIndex.end()) {
        result = it->second;
        return true;
    }
    else {
        return false;
    }
}


void indexVBO(
    std::vector<glm::vec3>& in_vertices, std::vector<glm::vec3>& in_normals, std::vector<glm::vec2>& in_uvs, std::vector<Vertex>& vecVertex, std::vector<unsigned int>& vecIndices
    ) {
    std::map<Vertex, unsigned int> mapVertexOutIndex;

    if (!in_uvs.size())
        in_uvs.resize(in_vertices.size(), {0.0f, 0.0f});

    // For each input vertex
    for (size_t i = 0; i < in_vertices.size(); i++) {

        Vertex vertex = Vertex(in_vertices[i], in_normals[i], in_uvs[i]);

        // Try to find a similar vertex in out_XXXX
        unsigned int index;
        bool found = findSimilarVertex(vertex, mapVertexOutIndex, index);

        if (found) { // A similar vertex is already in the VBO, use it instead !
            vecIndices.push_back(index);
        }
        else { // If not, it needs to be added in the output data.
            vecVertex.push_back(vertex);

            unsigned int newindex = (unsigned int)vecVertex.size() - 1;
            vecIndices.push_back(newindex);

            mapVertexOutIndex[vertex] = newindex;
        }
    }
}

Model::~Model()
{
    for (auto i : vecMeshes)
    {
        delete i;
    }
   /*
   for (auto i : vecMaterials)
    {
        delete i;
    }
    */
    for (auto i : vecTextures)
    {
        delete i;
    }
}

Model::Model(const std::string & objPath, const bool LoadTextures, glm::vec3 newPosition) : modelPath(objPath), modelDirectory(modelPath.substr(0, modelPath.find_last_of('/'))), position(newPosition) {
    LoadMTL();
    if (LoadTextures)
        LoadWithTextures();
    else {
            OwnLoad();
    }
}


bool Model::LoadMTL()
{
    std::ifstream mtlFile(modelPath.substr(0, modelPath.length() - 3) + "mtl");

    if (!mtlFile.is_open()) {
        std::cout << "ERROR can't load .mtl file!\nLoading OBJECT without materials.\n" << std::endl;
        return false;
    }

    std::string line;
    line.reserve(32);
    unsigned int matCount = 0;

    // get material count, so skip first line
    std::getline(mtlFile, line);
    std::getline(mtlFile, line);

    matCount = std::stoi(line.substr(line.find_last_of(' ') + 1));
    vecMaterials.reserve(matCount);

    // skipping next empty line
    std::getline(mtlFile, line);

    Material* mat = nullptr;
    matCount = 0;
    unsigned short colorType = 0;

    while (std::getline(mtlFile,line))
    {
        if (line.substr(0,3) == "new") {
            mat = new Material;
            mapMaterials[line.substr(line.find_first_of(' ') + 1)] = matCount++;
        }
        else if (line.empty()) {
            colorType = 0;
            vecMaterials.push_back(mat);
        }
        else if (line.find("Ns") != line.npos) {
            mat->shininess = std::stof(line.substr(3));
        }
        else if (line.find("Ka") != line.npos || line.find("Kd") != line.npos || line.find("Ks") != line.npos) {
            parseFloats(mat, line.substr(3), colorType);
        }        
    }

    vecMaterials.push_back(mat);
    
    mtlFile.close();
    return true;
}

// ############### Own Object Loader with just MTL ####################

bool Model::OwnLoad()
{
    std::ifstream modelFile(modelPath);

    if (!modelFile.is_open()) {
        std::cout << "ERROR can't open obj file!" << std::endl;
        return false;
    }

    std::string line;
    line.reserve(100);

    // output variables
    std::vector<Vertex> outVertex;
    std::vector<unsigned int> outIndices;
    Material* outMaterial = nullptr;
    // variables for loading one Mesh
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    // variables for loading whole obj and holding indices positions
    std::vector<glm::vec3> tmpVertices;
    std::vector<glm::vec3> tmpNormals;
    std::vector<glm::vec2> tmpUvs;

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    std::string materialName;
    const std::string usemtl = "usemtl ";

    // skipping lines until first mesh
    while (line[0] != 'o')
        std::getline(modelFile, line);

    //      v    /   vt  /  vn
    // f 1444038/1453465/1632014 1444074/1453466/1632014 1444077/1453467/1632014
    // musim nacist ty faces ktery nemaji stejny cely indices vertexu, texture coords a normals
    // nacist nejdriv klasicky vertexy, normaly a souradnice a pak to dat do Vertices podle danych indices
    // pricemz si nechat vektor Indices stranou pro EBO
    while (std::getline(modelFile, line)) {
        if (line.find('#') != line.npos)
            continue;
        else if (line[0] == 'o') {  // predindexace z tmpvertices do vyslednychvertices a pote ziskani indexu

            // assign by indices and clear unnecessary vectors for next mesh load
            for (size_t i = 0; i < vertexIndices.size(); i++) 
            {
                vertices.push_back(tmpVertices[vertexIndices[i] - 1]);
                normals.push_back(tmpNormals[normalIndices[i] - 1]);
                if(uvIndices.size())
                    uvs.push_back(tmpUvs[uvIndices[i] - 1]);
            }

            indexVBO(vertices, normals, uvs, outVertex, outIndices);
            vecMeshes.push_back(new Mesh(outVertex, outIndices, outMaterial));

            clearVectors(vertexIndices, normalIndices, uvIndices);
            clearVectors(vertices, normals, uvs);
            outVertex.clear();
            outIndices.clear();
            outMaterial = nullptr;
        }
        else if (line[0] == 'v' && line[1] != 't' && line[1] != 'n' ) {  // get vertex
            glm::vec3 vec;
            parseVec3Floats(line.substr(2), vec);
            tmpVertices.push_back(vec);
        } 
        else if (line[0] == 'v' && line[1] == 't') {  // get vertex texture coordinate
            glm::vec2 vec;
            parseVec2Floats(line.substr(3), vec);
            tmpUvs.push_back(vec);
        } 
        else if (line[0] == 'v' && line[1] == 'n') {  // get vertex normals
            glm::vec3 vec;
            parseVec3Floats(line.substr(3), vec);
            tmpNormals.push_back(vec);
        } 
        else if (line[0] == 'f') { // get face
            parseFace(line.substr(2), vertexIndices, normalIndices, uvIndices);
        } 
        else if (line.find(usemtl) != line.npos) { // get material and push point it to vector if it was loaded
            materialName = line.substr(usemtl.length(), line.length() - usemtl.length());

            auto it = mapMaterials.find(materialName);
            if (it != mapMaterials.end()) {
                outMaterial = vecMaterials[mapMaterials[materialName]];
            }
        }
    }

    for (size_t i = 0; i < vertexIndices.size(); i++)
    {
        vertices.push_back(tmpVertices[vertexIndices[i] - 1]);
        normals.push_back(tmpNormals[normalIndices[i] - 1]);
        uvs.push_back(tmpUvs[uvIndices[i] - 1]);
    }

    indexVBO(vertices, normals, uvs, outVertex, outIndices);
    vecMeshes.push_back(new Mesh(outVertex, outIndices, outMaterial));

    clearVectors(vertexIndices, normalIndices, uvIndices);
    clearVectors(vertices, normals, uvs);
    clearVectors(tmpVertices, tmpNormals, tmpUvs);
    outVertex.clear();
    outIndices.clear();
    
    modelFile.close();
    return true;
}

void Model::Draw(Shader& shader,const float size, glm::mat4& View, const glm::mat4& Projection, glm::vec3& cameraPos, glm::vec3& cameraFront) const {

   // if (position.x + size < cameraPos.x || position.x - size > cameraPos.x 
   //     || position.y + size < cameraPos.y || position.y - size > cameraPos.y
   //     || position.z + size < cameraPos.z || position.z - size > cameraPos.z);

    // Collision testing
    if (glm::distance(cameraPos, position) <= size*10) {
        //std::cout << glm::distance(cameraPos, position) << std::endl;
        auto tmpPosition = cameraPos - position;
        if (tmpPosition.x < 0)
            cameraPos.x = -size * 10.0f;
        else
            cameraPos.x = size * 10.0f;
        if (tmpPosition.y < 0)
            cameraPos.y = -size * 10.0f;
        else
            cameraPos.y = size * 10.0f;
        if (tmpPosition.z < 0)
            cameraPos.z = -size * 10.0f;
        else
            cameraPos.z = size * 10.0f;

        View = glm::lookAt(cameraPos, cameraPos + cameraFront , glm::vec3(0.0, 1.0, 0.0));
    }
    shader.Bind();

    // Init shader variables for Model
    glm::mat4 Model = glm::translate(glm::mat4(1.0f), position);
    Model = glm::scale(Model, glm::vec3(size, size, size));
    glm::mat3 NormalMatrix = glm::mat3(glm::transpose(glm::inverse(Model)));
    shader.SetUniformMatrix4fv("Model", Model);
    shader.SetUniformMatrix4fv("View", View);
    shader.SetUniformMatrix4fv("Projection", Projection);
    shader.SetUniformMatrix3fv("NormalizationMatrix", NormalMatrix);
    shader.SetUniform4f("fogParam.fogColor", glm::vec4(0.5, 0.5, 0.5, 1.0));
    shader.SetUniform1f("fogParam.fDensity", glm::distance(position, cameraPos) / 100.0);

    // Drawing each Mesh
    for (const auto i : vecMeshes)
    {
        (*i).Draw(shader);
    }

    shader.Unbind();
}


void Model::LoadWithTextures()
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR can't load OBJECT properly" << importer.GetErrorString() << std::endl;
		return;
	}

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        processMesh(mesh, scene);
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.emplace_back(vertex.Position, vertex.Normal, vertex.TexCoords);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    Material* outMaterial = nullptr;
    std::string matName = std::string(material->GetName().C_Str());
    auto it = mapMaterials.find(matName);
    if (it != mapMaterials.end()) {
        outMaterial = vecMaterials[mapMaterials[matName]];
    }

    vecMeshes.push_back(new Mesh(vertices, indices, textures, outMaterial));
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < vecTextures.size(); j++)
        {
            if ((((*vecTextures[j]).path).compare(str.C_Str())) == 0)
            {
                textures.push_back(*vecTextures[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture* texture = new Texture;
            (*texture).id = TextureFromFile(str.C_Str(), modelDirectory);
            (*texture).type = typeName;
            (*texture).path = str.C_Str();
            textures.push_back(*texture);
            vecTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // use linear blend of 2 mipmaps when texture is far
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // use linear filtering when texture closer
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
      //  glPixelStorei(GL_PACK_ALIGNMENT, 1);
      //  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

glm::vec3 Model::getPosition() const
{
    return position;
}

ModelAnimation::ModelAnimation(const unsigned short cntFrames) : numFrames(cntFrames), currFrame(0)
{}

void ModelAnimation::setFrame(unsigned int time, const int relFrameTime)
{
    currFrame = time / frameMs;
    currFrame %= numFrames;
    relativeTime = float(relFrameTime) / float(numFrames);
}

DynamicModel::DynamicModel(const std::string& objPath, const bool LoadTextures, glm::vec3 newPosition, glm::vec3 newDirection, float newSpeed,float newStartTime)
    : Model(objPath, LoadTextures, newPosition), direction(newDirection), speed(newSpeed), startTime(newStartTime)
{}

void DynamicModel::DrawDynamic(Shader& shader, const glm::mat4& Model, const glm::mat4& View, const glm::mat4& Projection, const glm::vec3 cameraPos) const
{
    shader.Bind();
    glm::mat3 NormalMatrix = glm::mat3(glm::transpose(glm::inverse(Model)));
    shader.SetUniformMatrix4fv("Model", Model);
    shader.SetUniformMatrix4fv("View", View);
    shader.SetUniformMatrix4fv("Projection", Projection);
    shader.SetUniformMatrix3fv("NormalizationMatrix", NormalMatrix);
    //shader.SetUniform4f("fogParam.fogColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    //shader.SetUniform1f("fogParam.fDensity", glm::distance(position, cameraPos) / 100.0);

    for (const auto&  i : vecMeshes)
    {
        (*i).Draw(shader);
    }

    shader.Unbind();
}

glm::vec3 DynamicModel::getDirection() const
{
    return direction;
}

void DynamicModel::setDirection(glm::vec3 newDir)
{
    direction = newDir;
}

glm::vec3 DynamicModel::getPosition() const
{
    return position;
}

void DynamicModel::setPosition(const glm::vec3 newPos)
{
    position = newPos;
}

float DynamicModel::getSpeed() const
{
    return speed;
}

void DynamicModel::setSpeed(const float newSpeed)
{
    speed = newSpeed;
}
