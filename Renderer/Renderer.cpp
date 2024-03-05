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
    // for shaders (ifs)
        // use program
        // for objects
            // lock vao
            // send uniforms
            // send light data to shader
            // send material data to shader
            // draw
    
    for(pair<Shaders, vector<Mesh*>> p: m_shaderDic)
    {
        switch (p.first)
        {
        case color:
            // p.second[0]->getShader()->useProgram();
                for(Mesh* mesh: p.second)
                {
                    mesh->draw(projection, view, m_lightManager);
                }
            // glUseProgram(0);
            break;

        case blinnPhong:
            // p.second[0]->getShader()->useProgram();
                for(Mesh* mesh: p.second)
                {
                    mesh->draw(projection, view, m_lightManager);
                }
            // glUseProgram(0);
            break;

        case GGX:
            // p.second[0]->getShader()->useProgram();
                for(Mesh* mesh: p.second)
                {
                    mesh->draw(projection, view, m_lightManager);
                }
            // glUseProgram(0);
            break;

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
