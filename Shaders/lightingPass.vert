#version 150 core

in vec2 in_Vertex;
in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(in_Vertex.x, in_Vertex.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  