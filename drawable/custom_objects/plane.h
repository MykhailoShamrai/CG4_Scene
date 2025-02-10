#ifndef PLANE_H
#define PLANE_H

#include "../mesh/mesh.h"

class Plane: public Mesh
{
public:
    unsigned int width = 1000;
    unsigned int height = 1000;
    unsigned int div = 100;
    Plane()
    {
        std::vector<Vertex> vertices = std::vector<Vertex>();
        float triangleSide = static_cast<float>(width) / div;
        for (int i = 0; i <= div; i++)
        {
            for (int j = 0; j <= div; j++)
            {
                Vertex vertex;
                // - because of z direction in opengl
                glm::vec3 pos = glm::vec3(i * triangleSide, 0, j * -triangleSide);
                vertex.Position = pos;
                glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
                vertex.Normal = normal;
                // Scaled positions of vertices. I'll use repeated texture
                glm::vec2 texCoords = 0.1f * glm::vec2(pos.x, pos.y);
                this->Vertices.push_back(vertex);
            }
        }

        // Indices
        for (int i = 0; i < div; i++)
        {
            for (int j = 0; j < div; j++)
            {
                int index = i * (div + 1) + j;
                // Top triangles
                this->Indices.push_back(index);
                this->Indices.push_back(index + (div + 1) + 1);
                this->Indices.push_back(index + (div + 1));
                // Bottom
                this->Indices.push_back(index);
                this->Indices.push_back(index + 1);
                this->Indices.push_back(index + (div + 1) + 1);
            }
        }

        // Hardcoded path for my custom object
        std::string directory = "";

    };
};

#endif //PLANE_H
