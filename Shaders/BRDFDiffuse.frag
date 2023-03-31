#version 150 core

in vec3 ambient;
in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;

out vec4 out_Color;

//uniform vec3 lightPos;

void main()
{
    vec3 lightPos = vec3(1,10,1);
    vec3 lightColor = vec3(1);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient + diffuse) * objectColor;


    out_Color = vec4(result, 1.0);
}
