#include "Renderer.h"

#include "../Mesh/Mesh.h"

Renderer::Renderer(int width, int height, LightManager* lightManager): m_width(width), m_height(height), m_lightManager(lightManager) 
{
    m_gBufferShader = new Shader("Shaders/BRDF.vert", "Shaders/gBufferBlinnPhongGGX.frag");
    m_deferredShader = new Shader("Shaders/lightingPass.vert", "Shaders/lightingPass.frag");
    m_ssaoShader = new Shader("Shaders/lightingPass.vert", "Shaders/ssao.frag");
    m_ssaoBlurShader = new Shader("Shaders/lightingPass.vert", "Shaders/blur.frag");

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

    initGBuffer();
    initSSAO();
}

Renderer::~Renderer() 
{
    // glDeleteFramebuffers(1, m_gBuffer);
}

void Renderer::addMesh(Mesh *mesh, string fragShader)
{
    m_meshes.push_back(mesh);
}

void Renderer::render(mat4 &projection, mat4 &view)
{
    // geometry pass

    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};    
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_gBufferShader->useProgram();
    for(Mesh* mesh: m_meshes)
    {
        glBindVertexArray(mesh->getVaoId());

            sendTransforms(projection, view, mesh->getModelTransform(), m_gBufferShader->getProgramID());
            m_gBufferShader->sendMaterialToShader(mesh->getMaterial());
            glDrawBuffers(3, attachments);
            glDrawElements(GL_TRIANGLES, mesh->getTrianglesNb()*3, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);
    }
    // glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ssao pass

    glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexture);

    m_ssaoShader->useProgram();

    glUniform1i(glGetUniformLocation(m_ssaoShader->getProgramID(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(m_ssaoShader->getProgramID(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(m_ssaoShader->getProgramID(), "noise"), 2);

    glUniform3fv(glGetUniformLocation(m_ssaoShader->getProgramID(), "samples"), 64, &m_ssaoKernel[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_ssaoShader->getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

    glDrawBuffers(1, attachments);
    glBindVertexArray(m_quadVaoId);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ssao blur pass

    glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);

    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ssaoColorBuffer);

    m_ssaoBlurShader->useProgram();

    glUniform1i(glGetUniformLocation(m_ssaoShader->getProgramID(), "ssaoInput"), 0);

    glDrawBuffers(1, attachments);
    glBindVertexArray(m_quadVaoId);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // lighting pass

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_gColorSpec);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_ssaoBlurColorBuffer);
    
    m_deferredShader->useProgram();

        glUniform1i(glGetUniformLocation(m_deferredShader->getProgramID(), "gPosition"), 0);
        glUniform1i(glGetUniformLocation(m_deferredShader->getProgramID(), "gNormal"), 1);
        glUniform1i(glGetUniformLocation(m_deferredShader->getProgramID(), "gAlbedoSpec"), 2);
        glUniform1i(glGetUniformLocation(m_deferredShader->getProgramID(), "ssao"), 3);

        m_lightManager->sendDataToShader(m_deferredShader);

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

    // z buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // position buffer
    glGenTextures(1, &m_gPosition);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);

    // normal and shininess buffer
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

    glBindBuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initSSAO()
{
    uniform_real_distribution<float> randomFloats(0.0, 1.0);
    default_random_engine generator;

    for(int i = 0; i < 64; i++)
    {
        vec3 sample(randomFloats(generator) * 2 -1,
                    randomFloats(generator) * 2 -1,
                    randomFloats(generator));
        sample = normalize(sample);
        sample *= randomFloats(generator);

        float scale = (float)i / 64;
        scale = mix(0.1f, 1.0f, scale * scale);
        sample *= scale;

        m_ssaoKernel.push_back(sample);
    }

    for(int i = 0; i < 16; i++)
    {
        vec3 noise(randomFloats(generator) * 2 - 1,
                   randomFloats(generator) * 2 - 1,
                   0);
        m_ssaoNoise.push_back(noise);
    }

    glGenTextures(1, &m_ssaoNoiseTexture);
    glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &m_ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &m_ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);

    glGenTextures(1, &m_ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorBuffer, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &m_ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
    glGenTextures(1, &m_ssaoBlurColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_ssaoBlurColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurColorBuffer, 0);
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
    glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, value_ptr(view));
}
