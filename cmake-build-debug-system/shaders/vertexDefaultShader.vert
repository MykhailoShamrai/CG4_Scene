#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    // Strange
    vec4 temp = vec4(aPos, 1.0);
    temp = model * temp;
    gl_Position = projection * view * temp;
    fragPos = vec3(temp);
    texCoord = aTexCoord;
    normal = normalMatrix * aNormal;
}