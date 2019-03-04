#include "MeshEdge.h"

#include "MeshVertex.h"
#include "Mesh.h"

MeshEdge::MeshEdge(Mesh* mesh, MeshVertex* v1, MeshVertex* v2): m_vertex1(v1), m_vertex2(v2), m_mesh(mesh)
{
    if(mesh != nullptr)
    {
        mesh->pushEdge(this);
    }
}

MeshEdge::~MeshEdge()
{
    if(m_mesh != nullptr)
    {
        m_mesh->popEdge(this);
        m_mesh = nullptr;
    }

    m_vertex1 = nullptr;
    m_vertex2 = nullptr;
}