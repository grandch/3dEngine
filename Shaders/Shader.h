#ifndef DEF_SHADER
#define DEF_SHADER

#include <GLES3/gl3.h>

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Shader
{
    public:

        Shader();
        ~Shader();

        Shader(string vertexPath, string fragmentPath);
        Shader(Shader const &toCopyShader);

        Shader& operator=(Shader const &toCopyShader);

        GLuint getProgramID() const;

        bool load();
        bool shaderCompile(GLuint &shader, GLenum type, string const &filePath);

    private:

        GLuint m_vertexID;
        GLuint m_fragmentID;
        GLuint m_programID;

        string m_vertexPath;
        string m_fragmentPath;
};

#endif