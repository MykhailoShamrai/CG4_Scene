#version 330
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
void main()
{
    FragColor = texture(texture_diffuse1, texCoord);
}