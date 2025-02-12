#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "../drawable.h"
#include "mesh.h"
#include "shader.h"
#include "assimp/material.h"


/* Forward declarations */
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

class Model final : public virtual Drawable
{
public:
    Model(const std::string& path) { loadModel(path); };
    void Draw(const std::unordered_map<std::string, Shader> &shaders) override;
    void SetCustomMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) override;
    void SetDefaultMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) override;
    void SetSpecularAndShininess(const glm::vec3 &specular, const float &shininess) override;
    void SetDefaultSpecularAndShininess(const glm::vec3 &specular, const float &shininess) override;

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    Material loadMaterial(aiMaterial* mat);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif  // MODEL_H
