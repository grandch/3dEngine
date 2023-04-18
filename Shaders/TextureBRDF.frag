#version 150 core

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec2 out_uv;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularStrength;
uniform float ambientStrength;
uniform float shininess;
uniform mat4 view;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D roughnessTexture;

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
    return diff * vec3(texture(diffuseTexture, out_uv)) * att(lightPos, strength) * color;
}

vec3 specular(vec4 lightPos, vec3 color, float strength)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), pow(2, 7*float(texture(roughnessTexture, out_uv))));
    return float(texture(specularTexture, out_uv)) * spec * specularColor * att(lightPos, strength) * color;
}

void main()
{
    vec3 result = ambient + diffuse(view * vec4(4,10,4,1), vec3(1,0.95,0.9), 1.0) + specular(view * vec4(4,10,4,1), vec3(1,0.95,0.9), 1.0) + diffuse(view * vec4(-4,5,4,1), vec3(1,0.6,0.3), 0.4) + specular(view * vec4(-4,5,4,1), vec3(1,0.6,0.3), 0.4) + diffuse(view * vec4(0,5,-3,1), vec3(0.6,0.9,1), 0.6) + specular(view * vec4(0,5,-3,1), vec3(0.6,0.9,1), 0.6);
    out_Color = vec4(result, 1.0);
}
