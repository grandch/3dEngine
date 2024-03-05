#include "Renderer.h"

#include "../Mesh/Mesh.h"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::addMesh(Mesh *mesh, string fragShader)
{
    m_shaderDic[fragShader].push_back(mesh);
}

void Renderer::addShader(string fragShader, vector<Mesh *> meshes)
{
    for(Mesh* mesh: meshes)
    {
        m_shaderDic[fragShader].push_back(mesh);
    }
}

void Renderer::render()
{
    // for shaders (ifs)
        // use program
        // for objects
            // lock vao
            // send uniforms
            // send light data to shader
            // send material data to shader
            // draw
}
