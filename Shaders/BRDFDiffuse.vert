#version 150 core

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 ambient;
out vec3 normal;
out vec3 fragPos;
out vec3 objectColor;
out vec3 lightPos;

vec3 ambientColor()
{
    float ambientStrenght = 0.1;
    vec3 lightColor = vec3(1);
    vec3 ambient = lightColor * ambientStrenght;
    vec3 result = ambient * in_Color;
    return result;
}

void main()
{
    gl_Position = projection * model * view * vec4(in_Vertex, 1.0);

    lightPos = vec3(1,10,1);
    ambient = ambientColor();
    normal = vec3(mat3(transpose(inverse(model))) * in_normal);
    fragPos = vec3(model * view * vec4(in_Vertex, 1.0));
    objectColor = in_Color;
}
