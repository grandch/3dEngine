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

        void loadDiffuseTexture(const char* path);
        unsigned int getDiffuseTexture();

        void loadSpecularTexture(const char* path);
        unsigned int getSpecularTexture();

        void loadRoughnessTexture(const char* path);
        unsigned int getRoughnessTexture();

    private:

        GLuint m_vertexID;
        GLuint m_fragmentID;
        GLuint m_programID;

        string m_vertexPath;
        string m_fragmentPath;

        Material* m_material;
        unsigned int m_diffuseTexture;
        unsigned int m_specularTexture;
        unsigned int m_roughnessTexture;
};

#endif