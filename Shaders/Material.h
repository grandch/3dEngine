#ifndef MATERIAL
#define MATERIAL

#include <GLES3/gl3.h>
#include <glm/gtc/type_ptr.hpp>
#include "../ext/stb/stb_image.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace glm;
using namespace std;

enum Shaders {
    blinnPhong,
    GGX,
    color,
    invalid
};

class Material
{
    public:

        Material(Shaders shaderType, vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess);
        ~Material();

        Shaders getShaderType();

        void setMaterial(vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess);

        vec3 getDiffuseColor();
        vec3 getSpecularColor();
        float getSpecularStrength();
        float getShininess();

        bool hasDiffuseColorTexture();
        bool hasSpecularColorTexture();
        bool hasShininessTexture();
        bool hasSpecularStrengthTexture();

        void loadDiffuseColorTexture(const char* path);
        unsigned int getDiffuseColorTexture();

        void loadSpecularColorTexture(const char* path);
        unsigned int getSpecularColorTexture();

        void loadShininessTexture(const char* path);
        unsigned int getShininessTexture();

        void loadSpecularStrength(const char* path);
        unsigned int getSpecularStrengthTexture();

    private:

        vec3 m_diffuseColor;
        vec3 m_specularColor;
        float m_specularStrength;
        float m_shininess;

        bool m_hasDiffuseColorTexture = false;
        unsigned int m_diffuseColorTexture;
        bool m_hasSpecularColorTexture = false;
        unsigned int m_specularColorTexture;
        bool m_hasShininessTexture = false;
        unsigned int m_shininessTexture;
        bool m_hasSpecularStrengthTexture = false;
        unsigned int m_specularStrengthTexture;

        Shaders m_shaderType;
};

#endif