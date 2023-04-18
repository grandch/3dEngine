#version 150 core

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 objectColor;

void main()
{
    gl_Position = projection * view * model * vec4(in_Vertex, 1.0);
    objectColor = in_Color;
}
