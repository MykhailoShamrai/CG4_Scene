#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <string>
struct Texture
{
    unsigned int Id;
    std::string Type;
    std::string Path;
};

class TextureLoader
{
public:
    static unsigned int TextureFromFile(const std::string &filename, const std::string &directory);
};
#endif //TEXTURELOADER_H
