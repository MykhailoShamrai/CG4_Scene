#include "model.h"

#include <iostream>
#include <ostream>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "textureLoader.h"

void Model::Draw(const std::unordered_map<std::string, Shader> &shaders)
{
    setTransformationsForDrawable(shaders);
    for (auto &mesh : meshes)
    {
        mesh.Draw(shaders);
    }
}
void Model::SetCustomMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    Drawable::SetCustomMaterial(ambient, diffuse, specular, shininess);
    for (auto &mesh : meshes)
    {
        mesh.Material_ = customMaterial;
    }
}
void Model::SetSpecularAndShininess(const glm::vec3 &specular, const float &shininess)
{
    Drawable::SetSpecularAndShininess(specular, shininess);
    for (auto &mesh : meshes)
    {
        mesh.Material_ = customMaterial;
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate |
                aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        throw std::runtime_error("Error loading model");
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    // Start of processing
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Firstly - process all the meshes of a node (if any exists)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // TODO: I can create here parent-child hierarchy a little-bit later
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].Path.data(),
            str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        // If texture was loaded, then skip it
        if (!skip)
        {
            Texture texture;
            texture.Id = TextureLoader::TextureFromFile(std::string(str.C_Str()), directory);
            texture.Type = typeName;
            texture.Path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Material Model::loadMaterial(aiMaterial *mat)
{
    Material material;
    aiColor3D color(0.0f, 0.0f, 0.0f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Material mat;

    // All vertices of a mesh to vector
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.Position = vector;
        vector.x        = mesh->mNormals[i].x;
        vector.y        = mesh->mNormals[i].y;
        vector.z        = mesh->mNormals[i].z;
        vertex.Normal   = vector;
        if (mesh->mTextureCoords[0])  // Only if mesh contains texture coords
        {
            glm::vec2 vec;
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    // Process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material             = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        mat = loadMaterial(material);
    }
    // In both situations - custom color and only shininess passed, shininess will be != -1.0f
    // if (customMaterial.IsCustom)
       // mat = customMaterial;
    return Mesh(vertices, indices, textures, mat);
}





