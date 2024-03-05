#include "Renderer.h"

#include "../Mesh/Mesh.h"

Renderer::Renderer(LightManager* lightManager): m_lightManager(lightManager) {}

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

        case invalid:
            break;
        default:
            break;
        }
    }
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
