#version 150 core

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_normal;
in vec2 in_uv;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 ambientColor;
uniform float ambientStrength;

out vec3 ambient;
out vec3 normal;
out vec3 fragPos;
out vec3 objectColor;
out vec2 out_uv;
out vec3 lightPos;


vec3 ambientClr()
{
    vec3 ambient = ambientColor * ambientStrength;
    vec3 result = ambient * in_Color;
    return result;
}

void main()
{
    gl_Position = projection * model * view * vec4(in_Vertex, 1.0);

    lightPos = vec3(3,10,4);
    ambient = ambientClr();
    normal = vec3(mat3(transpose(inverse(model))) * in_normal);
    fragPos = vec3(model * view * vec4(in_Vertex, 1.0));
    objectColor = in_Color;
    out_uv = in_uv;
}
