#include "Shader.h"

Shader::Shader(): vertexID(0), fragmentID(0), programID(0), vertexPath(), fragmentPath()
{}

Shader::Shader(string vertexPath, string fragmentPath): vertexID(0), fragmentID(0), programID(0), vertexPath(vertexPath), fragmentPath(fragmentPath)
{}

Shader::Shader(Shader const &toCopyShader)
{
    vertexPath = toCopyShader.vertexPath;
    fragmentPath = toCopyShader.fragmentPath;

    load();
}

Shader& Shader::operator=(Shader const &toCopyShader)
{
    vertexPath = toCopyShader.vertexPath;
    fragmentPath = toCopyShader.fragmentPath;

    load();

    return *this;
}

Shader::~Shader()
{
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    glDeleteProgram(programID);
}


GLuint Shader::getProgramID() const
{
    return programID;
}

bool Shader::load()
{
    GLint linkError(0);

    if(glIsShader(vertexID) == GL_TRUE)
    {
        glDeleteShader(vertexID);
    }
    if(glIsShader(fragmentID) == GL_TRUE)
    {
        glDeleteShader(fragmentID);
    }
    if(glIsProgram(programID) == GL_TRUE)
    {
        glDeleteProgram(programID);
    }

    if(!shaderCompile(vertexID, GL_VERTEX_SHADER, vertexPath))
    {
        return false;
    }
    if(!shaderCompile(fragmentID, GL_FRAGMENT_SHADER, fragmentPath))
    {
        return false;
    }   

    programID = glCreateProgram();

    //associate the shaders
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);

    glBindAttribLocation(programID, 0, "in_Vertex");

    glBindAttribLocation(programID, 1, "in_Color");
    glBindAttribLocation(programID, 2, "in_TexCoord0");

    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &linkError);

    if(linkError != GL_TRUE)
    {
        GLint errorSize(0);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorSize);

        char *error = new char[errorSize + 1];

        glGetProgramInfoLog(programID, errorSize, &errorSize, error);
        error[errorSize] = '\0';

        cout << error << endl;

        delete[] error;
        glDeleteProgram(programID);

        return false;
    }

    return true;
}

bool Shader::shaderCompile(GLuint &shader, GLenum type, string const &filePath)
{
    shader = glCreateShader(type); //create the shader ID
    string line, code;
    GLint compileError(0);

    if(shader == 0)
    {
        cout << "Error : shader type " << type << " doesn't exist" << endl;
        return false;
    }

    ifstream file(filePath.c_str());

    if(!file)
    {
        cout << "Error : file " << filePath << " doesn't exist" << endl;
        glDeleteShader(shader);
        return false;
    }

    while(getline(file, line)) //read the code file
    {
        code += line + '\n';
    }

    file.close();

    const GLchar *codeString = code.c_str();

    glShaderSource(shader, 1, &codeString, 0);
    glCompileShader(shader);

    //compilation check
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileError); 
    if(compileError != GL_TRUE)
    {
        GLint errorSize(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);

        char *error = new char[errorSize + 1];

        glGetShaderInfoLog(shader, errorSize, &errorSize, error);
        error[errorSize] = '\0';

        cout << error << endl;

        delete[] error;
        glDeleteShader(shader);

        return false;
    }

    return true;
}