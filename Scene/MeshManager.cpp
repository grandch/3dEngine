#include "MeshManager.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

void MeshManager::addMesh(string name, Mesh *mesh)
{
    m_meshes.insert({name, mesh});
    mesh->loadMesh();
}

Mesh* MeshManager::getMesh(string name)
{
    return m_meshes[name];
}

void MeshManager::removeMesh(string name)
{
    m_meshes.erase(name);
}

void MeshManager::transformMesh(string name, mat4 transform)
{
    m_meshes[name]->transform(transform);
}

void MeshManager::draw(mat4 &projection, mat4 &view, LightManager *lightManager, float time)
{    
    for(auto i = m_meshes.begin(); i != m_meshes.end(); ++i)
    {
        i->second->draw(projection, view, lightManager, 0.1*time);
    }
}