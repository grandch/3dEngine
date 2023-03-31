#version 150 core

in vec3 objectColor;

out vec4 out_Color;

void main()
{
    out_Color = vec4(objectColor, 1.0);
}
