#version 330 core

struct Material
{
    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;

    sampler2D diffuseColorTexture;
    sampler2D specularColorTexture;
    sampler2D shininessTexture;
    sampler2D specularStrengthTexture;

    bool hasDiffuseColorTexture;
    bool hasSpecularColorTexture;
    bool hasShininessTexture;
    bool hasSpecularStrengthTexture;
};

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 out_uv;
in vec3 fragPos;
in vec3 normal;

uniform Material material;

void main()
{
    gPosition = fragPos;
    gNormal.rgb = normalize(normal);
    
    if(material.hasShininessTexture)
    {
        gNormal.a = float(texture(material.shininessTexture, out_uv));
    }
    else
    {
        gNormal.a = material.shininess;
    }

    if(material.hasDiffuseColorTexture)
    {
        gAlbedoSpec.rgb = texture(material.diffuseColorTexture, out_uv).rgb;
    }
    else
    {
        gAlbedoSpec.rgb = vec3(material.diffuseColor);
    }
    if(material.hasSpecularStrengthTexture)
    {
        gAlbedoSpec.a = float(texture(material.specularStrengthTexture, out_uv));
    }
    else
    {
        gAlbedoSpec.a = material.specularStrength;
    }
}