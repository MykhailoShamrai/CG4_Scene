#ifndef MESH_H
#define MESH_H
#include <string>
#include <unordered_map>
#include <vector>

#include "drawable.h"
#include "glm/detail/type_vec.hpp"
#include "shader.h"
#include "textureLoader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh
{
public:
    std::vector<Vertex> Vertices{};
    std::vector<unsigned int> Indices{};
    std::vector<Texture> Textures{};
    Material Material_;

    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
        const std::vector<Texture> &textures, const Material &material);
    void Draw(const std::unordered_map<std::string, Shader> &shaders);
private:
    unsigned int VAO{}, VBO{}, EBO{};
    void SetupMesh();
};

#endif //MESH_H
