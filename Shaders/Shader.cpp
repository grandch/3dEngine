#include "Shader.h"

Shader::Shader(): m_vertexID(0), m_fragmentID(0), m_programID(0), m_vertexPath(), m_fragmentPath()
{}

Shader::Shader(string vertexPath, string fragmentPath, vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess): m_vertexID(0), m_fragmentID(0), m_programID(0), m_vertexPath(vertexPath), m_fragmentPath(fragmentPath), m_material(new Material)
{
    m_material->diffuseColor = diffuseColor;
    m_material->specularColor = specularColor;
    m_material->specularStrength = specularStrength;
    m_material->shininess = shininess;
}

Shader::Shader(Shader const &toCopyShader)
{
    m_vertexPath = toCopyShader.m_vertexPath;
    m_fragmentPath = toCopyShader.m_fragmentPath;
    m_material = toCopyShader.m_material;

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

Material* Shader::getMaterial()
{
    return m_material;
}

void Shader::setMaterial(vec3 diffuseColor, vec3 specularColor, float specularStrength, float shininess)
{
    m_material->diffuseColor = diffuseColor;
    m_material->specularColor = specularColor;
    m_material->specularStrength = specularStrength;
    m_material->shininess = shininess;
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

void Shader::loadDiffuseColorTexture(const char* path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_material->diffuseColorTexture));
        glBindTexture(GL_TEXTURE_2D, m_material->diffuseColorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_material->hasDiffuseColorTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getDiffuseColorTexture()
{
    return m_material->diffuseColorTexture;
}

void Shader::loadSpecularColorTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_material->specularColorTexture));
        glBindTexture(GL_TEXTURE_2D, m_material->specularColorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_material->hasSpecularColorTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getSpecularColorTexture()
{
    return m_material->specularColorTexture;
}

void Shader::loadShininessTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_material->shininessTexture));
        glBindTexture(GL_TEXTURE_2D, m_material->shininessTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_material->hasShininessTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getShininessTexture()
{
    return m_material->shininessTexture;
}

void Shader::loadSpecularStrength(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &(m_material->specularStrengthTexture));
        glBindTexture(GL_TEXTURE_2D, m_material->specularStrengthTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_material->hasSpecularStrengthTexture = true;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getSpecularStrength()
{
    return m_material->specularStrengthTexture;
}

void Shader::sendMaterialToShader()
{ 
    glUniform1i(glGetUniformLocation(m_programID, "material.hasDiffuseColorTexture"), m_material->hasDiffuseColorTexture);
    if(m_material->hasDiffuseColorTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_material->diffuseColorTexture);
    } else {
        glUniform3fv(glGetUniformLocation(getProgramID(), "material.diffuseColor"), 1, value_ptr(m_material->diffuseColor));
    }
    
    glUniform1i(glGetUniformLocation(m_programID, "material.hasSpecularColorTexture"), m_material->hasSpecularColorTexture);
    if(m_material->hasSpecularColorTexture)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_material->specularColorTexture);
    } else {
        glUniform3fv(glGetUniformLocation(getProgramID(), "material.specularColor"), 1, value_ptr(m_material->specularColor));
    }
    
    glUniform1i(glGetUniformLocation(m_programID, "material.hasShininessTexture"), m_material->hasShininessTexture);
    if(m_material->hasShininessTexture)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_material->shininessTexture);
    } else {
        glUniform1f(glGetUniformLocation(getProgramID(), "material.shininess"), m_material->shininess);
    }
    
    glUniform1i(glGetUniformLocation(m_programID, "material.hasSpecularStrengthTexture"), m_material->hasSpecularStrengthTexture);
    if(m_material->hasSpecularStrengthTexture)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_material->specularStrengthTexture);
    } else {
        glUniform1f(glGetUniformLocation(getProgramID(), "material.specularStrength"), m_material->specularStrength);
    }
}
