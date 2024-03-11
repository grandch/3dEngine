#version 150 core

in vec2 weights;

out vec4 out_Color;

void main()
{
    out_Color = vec4(weights, 0, 1.0);
}