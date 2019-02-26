#ifndef DEF_SHADER
#define DEF_SHADER

//Windows
#ifdef WIN32
#include <GL/glew.h>

//Mac
#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>

//UNIX/Linux
#else
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>
#endif


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

        GLuint fragmentID;
        GLuint vertexID;
        GLuint programID;

        string vertexPath;
        string fragmentPath;
};

#endif