#version 150 core

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec3 lightPos;

out vec4 out_Color;

//uniform vec3 lightPos;

void main()
{
    vec3 lightColor = vec3(1);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    int shininess = 32;
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;



    vec3 result = (ambient + diffuse + specular) * objectColor;
    out_Color = vec4(result, 1.0);
}
