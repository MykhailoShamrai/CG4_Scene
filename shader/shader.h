#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // the program ID
    unsigned int ID;
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void Use() const;
    // utility uniform functions
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetMat4(const std::string &name, glm::mat4 value) const;
    void SetMat3(const std::string &name, glm::mat3 value) const;
    void SetVec3(const std::string &name, glm::vec3 value) const;
};

#endif //SHADER_H
