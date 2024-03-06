#ifndef DEF_SHADER
#define DEF_SHADER

#include <GLES3/gl3.h>
#include <glm/gtc/type_ptr.hpp>
#include "../ext/stb/stb_image.h"

#include <iostream>
#include <string>
#include <fstream>

#include "Material.h"

using namespace glm;
using namespace std;

class Shader
{
    public:

        Shader(string vertexPath, string fragmentPath);
        Shader(Shader const &toCopyShader);

        ~Shader();

        Shader& operator=(Shader const &toCopyShader);

        GLuint getProgramID() const;
        void useProgram();

        bool load();
        bool shaderCompile(GLuint &shader, GLenum type, string const &filePath);

        void sendMaterialToShader(Material* material);

    private:

        GLuint m_vertexID;
        GLuint m_fragmentID;
        GLuint m_programID;

        string m_vertexPath;
        string m_fragmentPath;
};

#endif