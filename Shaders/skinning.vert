#version 150 core

in vec3 in_Vertex;
in vec2 in_weights;

uniform mat4 boneA;
uniform mat4 boneB;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 weights;

void main()
{
    vec4 position = (in_weights.x * boneA) * vec4(in_Vertex, 1.0) + (in_weights.y * boneB) * vec4(in_Vertex, 1.0);
    gl_Position = projection * view * model * position;
    weights = in_weights;
}
