#version 150 core
out vec4 out_color;
  
in vec2 TexCoords;

uniform sampler2D positionTexture;
// uniform sampler2D normalTexture;
// uniform sampler2D albedoSpecTexture;

void main()
{ 
    out_color = vec4(vec3(texture(positionTexture, TexCoords)), 1);
    // out_color = vec4(TexCoords.x, TexCoords.y, 1., 1.);
}