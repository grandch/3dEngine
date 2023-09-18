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

void Shader::loadDiffuseTexture(const char* path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &m_diffuseTexture);
        glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getDiffuseTexture()
{
    return m_diffuseTexture;
}

void Shader::loadSpecularTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &m_specularTexture);
        glBindTexture(GL_TEXTURE_2D, m_specularTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getSpecularTexture()
{
    return m_specularTexture;
}

void Shader::loadRoughnessTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        glGenTextures(1, &m_roughnessTexture);
        glBindTexture(GL_TEXTURE_2D, m_roughnessTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

unsigned int Shader::getRoughnessTexture()
{
    return m_roughnessTexture;
}
