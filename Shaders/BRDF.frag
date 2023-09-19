#version 150 core

struct LightManager
{
    vec4 pointLightsLocations[10];
    vec3 pointLightsColors[10];
    int nbPointLights;
    vec3 ambientColor;
    float ambientStrength;
};

struct Material
{
    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;
};

in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;

uniform Material material;
uniform LightManager lightManager;

uniform mat4 view;

out vec4 out_Color;

float att(vec4 lightPos, float strength)
{
    float dist = distance(lightPos.xyz, fragPos);
    return strength / (1.0 + 0.022 * dist + 0.0019 * pow(dist, 2));
}

vec3 ambient()
{
    vec3 amb = lightManager.ambientColor * lightManager.ambientStrength;
    vec3 result = amb * objectColor;
    return result;
}

vec3 diffuse(vec4 lightPos, vec3 color, float strength)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * material.diffuseColor * att(lightPos, strength) * color;
}

vec3 specular(vec4 lightPos, vec3 color, float strength)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    return material.specularStrength * spec * material.specularColor * att(lightPos, strength) * color;
}

void main()
{
    vec3 result = ambient();
    
    for(int i = 0; i < lightManager.nbPointLights; i++)
    {
        result += diffuse(view * lightManager.pointLightsLocations[i], lightManager.pointLightsColors[i], 1.0);
        result += specular(view * lightManager.pointLightsLocations[i], lightManager.pointLightsColors[i], 1.0);
    }
    
    out_Color = vec4(result, 1.0);
}
