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

    sampler2D diffuseColorTexture;
    sampler2D specularColorTexture;
    sampler2D shininessTexture;
    sampler2D specularStrengthTexture;

    bool hasDiffuseColorTexture;
    bool hasSpecularColorTexture;
    bool hasShininessTexture;
    bool hasSpecularStrengthTexture;
};

in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec2 out_uv;

uniform Material material;
uniform LightManager lightManager;
uniform vec3 eye;

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

vec3 diffuse()
{
    if(material.hasDiffuseColorTexture)
    {
        return vec3(texture(material.diffuseColorTexture, out_uv)) / pi;
    }
    return material.diffuseColor / pi;
}

int heaviside(x)
{
    if(x > 0)
        return 1;
    return 0;
}

vec3 specular(lightPos, alpha)
{
    l = fragPos - lightPos;
    // get eyePos
    v = fragPos - eyePos;
    h = normalize(l + V);
    G = (2*length(dot(normal, l))*heaviside(dot(h, l))) / (length(dot(normal, l)) + sqrt(alpha*alpha + pow((1-alpha), 2)*pow(dot(normal, l), 2)));
    G *= (2*length(dot(normal, v))*heaviside(dot(h, v))) / (length(dot(normal, v)) + sqrt(alpha*alpha + pow((1-alpha), 2)*pow(dot(normal, v), 2)));

    V = G / (4 * length(dot(normal, l)) * length(dot(normal, v)));

    D = (alpha * alpha * heaviside(dot(normal, h))) / (pi * pow((pow(dot(normal, h), 2)*(alpha*alpha -1) + 1), 2));

    if(material.hasSpecularColorTexture)
    {
        return texture(material.specularColorTexture, out_uv) * V * D;
    }
    return material.specularColor * V * D;
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
