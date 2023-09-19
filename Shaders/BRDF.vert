#version 150 core

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 normal;
out vec3 fragPos;
out vec3 objectColor;

void main()
{
    gl_Position = projection *  view * model * vec4(in_Vertex, 1.0);

    normal = mat3(transpose(inverse(model))) * in_normal, 0.0;
    fragPos = vec3(view * model * vec4(in_Vertex, 1.0));
    objectColor = in_Color;
}
