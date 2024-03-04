#include "PointLight.h"

PointLight::PointLight(vec4 location, vec3 color): m_location(location), m_color(color), m_shader("Shaders/depthShader.vert", "Shaders/empty.frag", vec3(0), vec3(0), 0, 0)
{
    glGenFramebuffers(1, &m_depthMapFBO);

    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffers(1, GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mat4 lightProjection = perspective(70.0, (double) 1, 1.0, 100.0);
    mat4 lightView = lookAt(vec3(m_location), vec3(0.f), vec3(0.f,1.f,0.f));
    m_lightSpaceMatrix = lightProjection * lightView;

    m_shader.load();
}

vec4 PointLight::getLocation()
{
    return m_location;
}

vec3 PointLight::getColor()
{
    return m_color;
}

void PointLight::setLocation(vec4 location)
{
    m_location = location;
}

void PointLight::setColor(vec3 color)
{
    m_color = color;
}

void PointLight::shadowMap(MeshManager* meshes)
{
    vector<pair<string, Mesh*>> meshesList = meshes->getMeshes();
    glUseProgram(m_shader.getProgramID());
        glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "lightSpaceMatrix"), 1, GL_FALSE, value_ptr(m_lightSpaceMatrix));
        glViewport(0,0,1024,1024);
        glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            for(pair<string, Mesh*> p : meshesList)
            {
                glBindVertexArray(p.second->getVaoId());                
                    glDrawElements(GL_TRIANGLES, p.second->getTriangles().size()*3, GL_UNSIGNED_SHORT, 0);
                glBindVertexArray(0);
            }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
}