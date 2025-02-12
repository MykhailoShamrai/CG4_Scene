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
#define NUMBER_POINT_LIGHTS 2
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 directionAfter;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
};
#define NUMBER_SPOT_LIGHTS 1
uniform SpotLight spotLights[NUMBER_SPOT_LIGHTS];

struct LightResult {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;
in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewerPos;
uniform bool day;

LightResult LightCalculation(vec3 ambientLight, vec3 diffuseLight, vec3 specularLight,
                             vec3 normal, vec3 ambientMaterial, vec3 diffuseMaterial, vec3 specularMaterial, float shininess,
                             vec3 lightDir, vec3 viewDir)
{
    vec3 reflectDir = reflect(lightDir, normal);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),
                     shininess);

    LightResult res;
    res.ambient = ambientLight * ambientMaterial;
    res.diffuse = diffuseLight * diffuseMaterial * diff;
    res.specular = specularLight * specularMaterial * spec;
    return(res);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 materialAmbient = vec3(material.ambient);
    vec3 materialDiffuse = vec3(material.diffuse);
    vec3 materialSpecular = vec3(material.specular);
    float shininess = material.shininess;
    LightResult res = LightCalculation(light.ambient, light.diffuse, light.specular, normal,
                                       materialAmbient, materialDiffuse, materialSpecular, shininess, lightDir, viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    res.ambient *= attenuation;
    res.specular *= attenuation;
    res.diffuse *= attenuation;
    return (res.ambient + res.diffuse + res.specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 materialAmbient = vec3(material.ambient);
    vec3 materialDiffuse = vec3(material.diffuse);
    vec3 materialSpecular = vec3(material.specular);
    float shininess = material.shininess;

    LightResult res = LightCalculation(light.ambient, light.diffuse, light.specular, normal,
                                       materialAmbient, materialDiffuse, materialSpecular, shininess, lightDir, viewDir);

    return (res.ambient + res.diffuse + res.specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 materialAmbient = vec3(material.ambient);
    vec3 materialDiffuse = vec3(material.diffuse);
    vec3 materialSpecular = vec3(material.specular);
    float shininess = material.shininess;
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    LightResult res;
    vec3 resultingColor;
    if (theta > light.outerCutOff)
    {
        res = LightCalculation(light.ambient, light.diffuse, light.specular, normal,
                               materialAmbient, materialDiffuse, materialSpecular, shininess, lightDir, viewDir);
        resultingColor = (res.ambient + res.diffuse * intensity + res.specular * intensity);
    }
    else
    {
        resultingColor = vec3(0.0f, 0.0f, 0.0f);
    }
    return resultingColor;
}


void main()
{
    // Normal vector
    vec3 viewerDir = normalize(fragPos - viewerPos);
    vec3 norm = normalize(normal);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    if (day)
        result += CalcDirLight(dirLight, norm, viewerDir);
    for (int i = 0; i < NUMBER_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragPos, viewerDir);
    for (int i = 0; i < NUMBER_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, fragPos, viewerDir);
    FragColor = vec4(result, 1.0);
}