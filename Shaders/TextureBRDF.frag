#version 150 core

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec3 lightPos;
in vec2 out_uv;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularStrength;
uniform float ambientStrength;
uniform float shininess;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D roughnessTexture;

out vec4 out_Color;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(diffuseTexture, out_uv));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), pow(2, 7*float(texture(roughnessTexture, out_uv))));
    vec3 specular = 1.5 * float(texture(specularTexture, out_uv)) * spec * specularColor;

    vec3 result = (ambient + diffuse + specular);
    out_Color = vec4(result, 1.0);
}
