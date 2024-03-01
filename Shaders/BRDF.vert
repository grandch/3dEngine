#version 150 core

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_normal;
in vec2 in_uv;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 normal;
out vec3 fragPos;
out vec3 objectColor;
out vec2 out_uv;

void main()
{
    gl_Position = projection *  view * model * vec4(in_Vertex, 1.0);

    normal = mat3(transpose(inverse(view * model))) * in_normal;
    fragPos = vec3(view * model * vec4(in_Vertex, 1.0));
    objectColor = in_Color;
    out_uv = in_uv;
}
