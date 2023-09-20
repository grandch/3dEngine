#include "MeshManager.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

void MeshManager::addMesh(char *name, Mesh *mesh)
{
    m_meshes.insert({name, mesh});
}

void MeshManager::removeMesh(char *name)
{
    m_meshes.erase(name);
}

void MeshManager::transformMesh(char *name, mat4 transform)
{
    m_meshes[name]->translate(transform);
}

void MeshManager::draw(mat4 &projection, mat4 &view, LightManager *lightManager)
{    
    for(auto i = m_meshes.begin(); i != m_meshes.end(); ++i)
    {
        i->second->draw(projection, view, lightManager);
    }
}