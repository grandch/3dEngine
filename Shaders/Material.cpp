#include "Material.h"

Material::Material(Shaders shaderType, vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess): m_shaderType(shaderType), m_diffuseColor(diffuseColor), m_specularColor(specularColor), m_specularStrength(specularStrength), m_shininess(shininess)
{}

Material::~Material()
{}

Shaders Material::getShaderType()
{
    return m_shaderType;
}

void Material::setMaterial(vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess)
{
    diffuseColor = diffuseColor;
    specularColor = specularColor;
    specularStrength = specularStrength;
    shininess = shininess;
}

vec3 Material::getDiffuseColor()
{
    return m_diffuseColor;
}

vec3 Material::getSpecularColor()
{
    return m_specularColor;
}

float Material::getSpecularStrength()
{
    return m_specularStrength;
}

float Material::getShininess()
{
    return m_shininess;
}

bool Material::hasDiffuseColorTexture()
{
    return m_hasDiffuseColorTexture;
}

bool Material::hasSpecularColorTexture()
{
    return m_hasSpecularColorTexture;
}

bool Material::hasShininessTexture()
{
    return m_hasShininessTexture;
}

bool Material::hasSpecularStrengthTexture()
{
    return m_hasSpecularStrengthTexture;
}

void Material::loadDiffuseColorTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_diffuseColorTexture));
        glBindTexture(GL_TEXTURE_2D, m_diffuseColorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_hasDiffuseColorTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Material::getDiffuseColorTexture()
{
    return m_diffuseColorTexture;
}

void Material::loadSpecularColorTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_specularColorTexture));
        glBindTexture(GL_TEXTURE_2D, m_specularColorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_hasSpecularColorTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Material::getSpecularColorTexture()
{
    return m_specularColorTexture;
}

void Material::loadShininessTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_shininessTexture));
        glBindTexture(GL_TEXTURE_2D, m_shininessTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_hasShininessTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Material::getShininessTexture()
{
    return m_shininessTexture;
}

void Material::loadSpecularStrength(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_specularStrengthTexture));
        glBindTexture(GL_TEXTURE_2D, m_specularStrengthTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_hasSpecularStrengthTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Material::getSpecularStrengthTexture()
{
    return m_specularStrengthTexture;
}