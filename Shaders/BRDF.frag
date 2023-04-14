#version 150 core

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec3 lightPos;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularStrength;
uniform float ambientStrength;
uniform float shininess;

out vec4 out_Color;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * specularColor;

    vec3 result = (ambient + diffuse + specular);
    out_Color = vec4(result, 1.0);
}
