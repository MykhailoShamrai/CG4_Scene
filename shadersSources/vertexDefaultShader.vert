#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;
out vec3 fragPosInViewSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    // Strange
    vec4 temp = vec4(aPos, 1.0);
    temp = model * temp;
    fragPos = vec3(temp);
    temp = view * temp;
    fragPosInViewSpace = vec3(temp);
    gl_Position = projection * temp;
    texCoord = aTexCoord;
    normal = normalMatrix * aNormal;
}