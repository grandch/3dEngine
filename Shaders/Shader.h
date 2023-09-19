#ifndef DEF_SHADER
#define DEF_SHADER

#include <GLES3/gl3.h>
#include <glm/gtc/type_ptr.hpp>
#include "../lib/stb_image.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace glm;
using namespace std;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;

    bool hasDiffuseColorTexture = false;
    unsigned int diffuseColorTexture;
    bool hasSpecularColorTexture = false;
    unsigned int specularColorTexture;
    bool hasShininessTexture = false;
    unsigned int shininessTexture;
    bool hasSpecularStrengthTexture = false;
    unsigned int specularStrengthTexture;
};

class Shader
{
    public:

        Shader();
        ~Shader();

        Shader(string vertexPath, string fragmentPath, vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess);
        Shader(Shader const &toCopyShader);

        Shader& operator=(Shader const &toCopyShader);

        GLuint getProgramID() const;
        Material* getMaterial();
        void setMaterial(vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess);

        bool load();
        bool shaderCompile(GLuint &shader, GLenum type, string const &filePath);

        void loadDiffuseColorTexture(const char* path);
        unsigned int getDiffuseColorTexture();

        void loadSpecularColorTexture(const char* path);
        unsigned int getSpecularColorTexture();

        void loadShininessTexture(const char* path);
        unsigned int getShininessTexture();

        void loadSpecularStrength(const char* path);
        unsigned int getSpecularStrength();

        void sendMaterialToShader();

    private:

        GLuint m_vertexID;
        GLuint m_fragmentID;
        GLuint m_programID;

        string m_vertexPath;
        string m_fragmentPath;

        Material* m_material;
};

#endif