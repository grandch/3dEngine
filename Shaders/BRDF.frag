#version 150 core

precision mediump float;

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;

uniform vec4 pointLightsLocations[10];
uniform vec3 pointLightsColors[10];
uniform int nbPointLights;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularStrength;
uniform float ambientStrength;
uniform float shininess;
uniform mat4 view;

out vec4 out_Color;

float att(vec4 lightPos, float strength)
{
    float dist = distance(lightPos.xyz, fragPos);
    return strength / (1.0 + 0.022 * dist + 0.0019 * pow(dist, 2));
}

vec3 diffuse(vec4 lightPos, vec3 color, float strength)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * diffuseColor * att(lightPos, strength) * color;
}

vec3 specular(vec4 lightPos, vec3 color, float strength)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularStrength * spec * specularColor * att(lightPos, strength) * color;
}

void main()
{
    vec3 result = ambient; 
    
    for(int i = 0; i < nbPointLights; i++)
    {
        result += diffuse(view * pointLightsLocations[i], pointLightsColors[i], 1.0);
        result += specular(view * pointLightsLocations[i], pointLightsColors[i], 1.0);
    }
    
    out_Color = vec4(result, 1.0);
}
