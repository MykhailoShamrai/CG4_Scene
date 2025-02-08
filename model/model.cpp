#include <model.h>

void Model::Draw(const Shader &shader)
{
    for (auto mesh : meshes)
    {
        mesh.Draw(shader);
    }
}
