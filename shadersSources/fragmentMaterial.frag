#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NUMBER_POINT_LIGHTS 1
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];

struct SpotLight
{
    vec3 Position;
    vec3 Direction;
    vec3 DirectionAfter;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float cutOff;
};
#define NUMBER_SPOT_LIGHTS 1
uniform SpotLight spotLights[NUMBER_SPOT_LIGHTS];

out vec4 FragColor;
in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewerPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),
                     material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(material.ambient);
    vec3 diffuse = light.diffuse * vec3(material.diffuse) * diff;
    vec3 specular = light.specular * vec3(material.specular) * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // Inverse, because we want have direction to light
    vec3 lightDir = normalize(-light.direction);
    // Cos from diffuse part
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(material.ambient);

    vec3 diffuse = light.diffuse * vec3(material.diffuse) * diff;
    // if (material.texture_specular1)
    vec3 specular = light.specular * vec3(material.specular) * spec;

    return (ambient + diffuse + specular);
}



void main()
{
    // Normal vector
    vec3 viewerDir = normalize(fragPos - viewerPos);
    vec3 norm = normalize(normal);
    vec3 result = CalcDirLight(dirLight, norm, viewerDir);
    for(int i = 0; i < NUMBER_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragPos, viewerDir);
    FragColor = vec4(result, 1.0);
}