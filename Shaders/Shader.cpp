#include "Shader.h"

Shader::Shader(string vertexPath, string fragmentPath): m_vertexID(0), m_fragmentID(0), m_programID(0), m_vertexPath(vertexPath), m_fragmentPath(fragmentPath)
{
    load();
}

Shader::Shader(Shader const &toCopyShader)
{
    m_vertexPath = toCopyShader.m_vertexPath;
    m_fragmentPath = toCopyShader.m_fragmentPath;

    load();
}

Shader& Shader::operator=(Shader const &toCopyShader)
{
    m_vertexPath = toCopyShader.m_vertexPath;
    m_fragmentPath = toCopyShader.m_fragmentPath;

    load();

    return *this;
}

Shader::~Shader()
{
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}


GLuint Shader::getProgramID() const
{
    return m_programID;
}

void Shader::useProgram()
{
    glUseProgram(m_programID);
}

bool Shader::load()
{
    GLint linkError(0);
    if(glIsShader(m_vertexID) == GL_TRUE)
    {
        glDeleteShader(m_vertexID);
    }
    if(glIsShader(m_fragmentID) == GL_TRUE)
    {
        glDeleteShader(m_fragmentID);
    }
    if(glIsProgram(m_programID) == GL_TRUE)
    {
        glDeleteProgram(m_programID);
    }

    if(!shaderCompile(m_vertexID, GL_VERTEX_SHADER, m_vertexPath))
    {
        return false;
    }
    if(!shaderCompile(m_fragmentID, GL_FRAGMENT_SHADER, m_fragmentPath))
    {
        return false;
    } 

    m_programID = glCreateProgram();
    
    //associate the shaders
    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);

    glBindAttribLocation(m_programID, 0, "in_Vertex");

    glBindAttribLocation(m_programID, 1, "in_Color");
    glBindAttribLocation(m_programID, 2, "in_TexCoord0");

    glLinkProgram(m_programID);

    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkError);

    if(linkError != GL_TRUE)
    {
        GLint errorSize(0);
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &errorSize);

        char *error = new char[errorSize + 1];

        glGetProgramInfoLog(m_programID, errorSize, &errorSize, error);
        error[errorSize] = '\0';

        cout << error << endl;

        delete[] error;
        glDeleteProgram(m_programID);

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
        cout << "Error : shader type " << type << endl;
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

void Shader::sendMaterialToShader(Material* material)
{ 
    glUniform1i(glGetUniformLocation(m_programID, "material.hasDiffuseColorTexture"), material->hasDiffuseColorTexture());
    if(material->hasDiffuseColorTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseColorTexture());
    } else {
        glUniform3fv(glGetUniformLocation(getProgramID(), "material.diffuseColor"), 1, value_ptr(material->getDiffuseColor()));
    }
    
    glUniform1i(glGetUniformLocation(m_programID, "material.hasSpecularColorTexture"), material->hasSpecularColorTexture());
    if(material->hasSpecularColorTexture())
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularColorTexture());
    } else {
        glUniform3fv(glGetUniformLocation(getProgramID(), "material.specularColor"), 1, value_ptr(material->getSpecularColor()));
    }
    
    glUniform1i(glGetUniformLocation(m_programID, "material.hasShininessTexture"), material->hasShininessTexture());
    if(material->hasShininessTexture())
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->getShininessTexture());
    } else {
        glUniform1f(glGetUniformLocation(getProgramID(), "material.shininess"), material->getShininess());
    }
    
    glUniform1i(glGetUniformLocation(m_programID, "material.hasSpecularStrengthTexture"), material->hasSpecularStrengthTexture());
    if(material->hasSpecularStrengthTexture())
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularStrengthTexture());
    } else {
        glUniform1f(glGetUniformLocation(getProgramID(), "material.specularStrength"), material->getSpecularStrength());
    }
}
