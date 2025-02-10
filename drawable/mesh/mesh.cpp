#include "mesh.h"

#include <glad/glad.h>

#include "textureLoader.h"

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Using .size() for vector to know ho manzy elements there are
    // TODO: May be I should change to GL_DYNAMIC_DRAW
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0],
        GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    // 3 because of Vec3 inside vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        static_cast<void *>(0));

    // Normal vectors for normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)(offsetof(Vertex, Normal)));

    // Vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void *)(offsetof(Vertex, TexCoords)));

    glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
    const std::vector<Texture> &textures, const Material& material):
    Vertices(vertices), Indices(indices), Textures(textures), Material_(material) { SetupMesh(); }

void Mesh::Draw(const std::unordered_map<std::string, Shader> &shaders)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    bool useTexture = false;
    bool useSpecular = false;
    for (unsigned int i = 0; i < Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = Textures[i].Type;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
            useTexture = true;
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++);
            useSpecular = true;
        }

        glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
    }
    Shader shader = shaders.at("programModelTextures");
    shader.Use();
    if (useTexture && !Material_.IsCustom)
    {
        // If there is texture, also it has specular map
        if (useSpecular)
        {
            for (unsigned int i = 0; i < specularNr; i++)
            {
                shader.SetInt("material.texture_specular" + std::to_string(i+1), i);
            }
        }
        // If there is a texture but not specular map. We should use material defined
        else
        {
            shader = shaders.at("programModelWithoutSpecular");
            shader.Use();
            shader.SetVec3("material.specular", Material_.Specular);
            shader.SetFloat("material.shininess", Material_.Shininess);
        }
        for (unsigned int i = 0; i < diffuseNr; i++)
        {
            shader.SetInt("material.texture_diffuse" + std::to_string(i+1), i);
        }
    }
    // If there is no texture, but only material or material is customly defined
    else
    {
        shader = shaders.at("programMaterials");
        shader.Use();
        shader.SetVec3("material.ambient", Material_.Ambient);
        shader.SetVec3("material.diffuse", Material_.Diffuse);
        shader.SetVec3("material.specular", Material_.Specular);
        shader.SetFloat("material.shininess", Material_.Shininess);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}