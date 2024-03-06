#include "Renderer.h"

#include "../Mesh/Mesh.h"

Renderer::Renderer(int width, int height, LightManager* lightManager): m_width(width), m_height(height), m_lightManager(lightManager) 
{
    m_gBufferShader = new Shader("Shaders/BRDF.vert", "Shaders/gBufferBlinnPhongGGX.frag");
    m_deferredShader = new Shader("Shaders/lightingPass.vert", "Shaders/lightingPass.frag");

    vector<float> quadVertices = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };	

    // make vbo
    glGenBuffers(1, &m_quadVboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size()*sizeof(GLfloat), quadVertices.data(), GL_STATIC_DRAW);

    // make vao
    glGenVertexArrays(1, &m_quadVaoId);
    glBindVertexArray(m_quadVaoId);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

Renderer::~Renderer() {}

void Renderer::addMesh(Mesh *mesh, string fragShader)
{
    m_shaderDic[resolveShader(fragShader)].push_back(mesh);
}

void Renderer::addShader(string fragShader, vector<Mesh *> meshes)
{
    for(Mesh* mesh: meshes)
    {
        m_shaderDic[resolveShader(fragShader)].push_back(mesh);
    }
}

void Renderer::render(mat4 &projection, mat4 &view)
{
    // // geometry pass

    // unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    // for(pair<Shaders, vector<Mesh*>> p: m_shaderDic)
    // {
    //     if(p.first == GGX)
    //     {
    //         m_gBufferShader->useProgram();
    //         for(Mesh* mesh: p.second)
    //         {
    //             glBindVertexArray(mesh->getVaoId());

    //                 sendTransforms(projection, view, mesh->getModelTransform(), m_gBufferShader->getProgramID());
    //                 m_gBufferShader->sendMaterialToShader(mesh->getMaterial());
    //                 glDrawBuffers(3, attachments);

    //             glBindVertexArray(0);
    //         }
    //         glUseProgram(0);
    //     }
    // }

    // // lighting pass

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_gPosition);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, m_gNormal);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, m_gColorSpec);
    
    m_deferredShader->useProgram();
        glBindVertexArray(m_quadVaoId);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    glUseProgram(0);
}

void Renderer::initGBuffer()
{
    // g buffer
    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    // position buffer
    glGenTextures(1, &m_gPosition);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);

    // normal buffer
    glGenTextures(1, &m_gNormal);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);

    // color and specular buffer
    glGenTextures(1, &m_gColorSpec);
    glBindTexture(GL_TEXTURE_2D, m_gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gColorSpec, 0);
}

Shaders Renderer::resolveShader(string input)
{
    if(input == "Shaders/BRDF.frag") return blinnPhong;
    if(input == "Shaders/BRDFmicroFacet.frag") return GGX;
    if(input == "Shaders/Color.frag") return color;
    return invalid;
}

void Renderer::sendTransforms(mat4 &projection, mat4 &view, mat4 model, GLuint programId)
{
    glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, value_ptr(projection));
}
