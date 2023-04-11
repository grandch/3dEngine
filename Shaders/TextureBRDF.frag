#version 150 core

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec3 lightPos;
in vec3 diffuse_color;
in vec3 specular_color;
in float shnn;
in float specStrength;
in vec2 out_uv;

uniform sampler2D uniTexture;

out vec4 out_Color;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(uniTexture, out_uv));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shnn);
    vec3 specular = specStrength * spec * specular_color;

    vec3 result = (ambient + diffuse + specular) * vec3(texture(uniTexture, out_uv));
    out_Color = vec4(result, 1.0);
}
