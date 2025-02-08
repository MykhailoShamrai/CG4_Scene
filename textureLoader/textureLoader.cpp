#include "textureLoader.h"

#include <glad/glad.h>
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

unsigned int TextureLoader::TextureFromFile(const std::string &filename, const std::string &directory)
{
    std::string path = directory + '/' + filename;

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents,
        0);
    if (data)
    {
        GLenum format = GL_RGB;
        switch (nrComponents)
        {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                throw std::runtime_error("Texture loading error! Something with format for collors");
        }

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        throw std::runtime_error(std::string(filename) + ": could not load image");
    }
    return textureId;
}
