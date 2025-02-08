#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>

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

    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
        const std::vector<Texture> &textures
    );
    void Draw(const Shader &shader);
private:
    unsigned int VAO{}, VBO{}, EBO{};
    void SetupMesh();
};

#endif //MESH_H
