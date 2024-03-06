#include "Renderer.h"

#include "../Mesh/Mesh.h"

Renderer::Renderer(int width, int height, LightManager* lightManager): m_width(width), m_height(height), m_lightManager(lightManager) 
{
    m_gBufferShader = new Shader("Shaders/BRDF.vert", "Shaders/gBufferBlinnPhongGGX.frag", vec3(0), vec3(0), 0, 0);
    // m_deferredShader = new Shader();
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
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    for(pair<Shaders, vector<Mesh*>> p: m_shaderDic)
    {
        switch (p.first)
        {
        case color:
        {
            Shader* shader = p.second[0]->getShader();
            shader->useProgram();
                for(Mesh* mesh: p.second)
                {
                    glBindVertexArray(mesh->getVaoId());
                        sendTransforms(projection, view, mesh->getModelTransform(), shader->getProgramID());
                        glDrawElements(GL_TRIANGLES, mesh->getTrianglesNb()*3, GL_UNSIGNED_SHORT, 0);
                    glBindVertexArray(0);
                    if(mesh->displayEdges())
                    {
                        glBindVertexArray(mesh->getEdgeVaoId());
                        // send uniforms
                        glLineWidth(1.5);
                        glDrawElements(GL_LINES,mesh->getEdgesNb()*2, GL_UNSIGNED_SHORT, 0);
                    glBindVertexArray(0);
                    }
                }
            glUseProgram(0);
            break;
        }

        case blinnPhong:
        {
            Shader* shader = p.second[0]->getShader();
            shader->useProgram();
                for(Mesh* mesh: p.second)
                {
                    glBindVertexArray(mesh->getVaoId());
                        sendTransforms(projection, view, mesh->getModelTransform(), shader->getProgramID());
                        m_lightManager->sendDataToShader(shader);
                        mesh->getShader()->sendMaterialToShader();
                        glDrawElements(GL_TRIANGLES, mesh->getTrianglesNb()*3, GL_UNSIGNED_SHORT, 0);
                    glBindVertexArray(0);
                    if(mesh->displayEdges())
                    {
                        glBindVertexArray(mesh->getEdgeVaoId());
                        // send uniforms
                        // send light data to shader
                        // send material data to shader
                        glLineWidth(1.5);
                        glDrawElements(GL_LINES,mesh->getEdgesNb()*2, GL_UNSIGNED_SHORT, 0);
                    glBindVertexArray(0);
                    }
                }
            glUseProgram(0);
            break;
        }

        case GGX:
        {
            Shader* shader = p.second[0]->getShader();
            shader->useProgram();
                for(Mesh* mesh: p.second)
                {
                    glBindVertexArray(mesh->getVaoId());

                        sendTransforms(projection, view, mesh->getModelTransform(), shader->getProgramID());
                        mesh->getShader()->sendMaterialToShader();
                        glDrawBuffers(3, attachments);

                        m_lightManager->sendDataToShader(shader);
                        glDrawElements(GL_TRIANGLES, mesh->getTrianglesNb()*3, GL_UNSIGNED_SHORT, 0);

                    glBindVertexArray(0);
                    if(mesh->displayEdges())
                    {
                        glBindVertexArray(mesh->getEdgeVaoId());
                        // send uniforms
                        // send light data to shader
                        // send material data to shader
                        glLineWidth(1.5);
                        glDrawElements(GL_LINES,mesh->getEdgesNb()*2, GL_UNSIGNED_SHORT, 0);
                    glBindVertexArray(0);
                    }
                }
            glUseProgram(0);
            break;
        }

        case invalid:
            break;
        default:
            break;
        }
    }
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
