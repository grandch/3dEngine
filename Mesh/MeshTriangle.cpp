#include "MeshTriangle.h"

#include "MeshHalfEdge.h"
#include "Mesh.h"
#include "MeshVertex.h"

MeshTriangle::MeshTriangle(Mesh* mesh, MeshVertex* v1, MeshVertex* v2, MeshVertex* v3): m_mesh(mesh)
{
    if(mesh != nullptr)
    {
        mesh->pushTriangle(this);
    }

    MeshHalfEdge* he12 = new MeshHalfEdge(mesh, v1, v2, this);
    MeshHalfEdge* he23 = new MeshHalfEdge(mesh, v2, v3, this);
    MeshHalfEdge* he31 = new MeshHalfEdge(mesh, v3, v1, this);

    //link the halfEdges
    he12->setNext(he23);
    he23->setNext(he31);
    he31->setNext(he12);

    m_halfEdge = he12;
}

MeshTriangle::~MeshTriangle()
{
    if(m_mesh != nullptr)
    {
        m_mesh->popTriangle(this);
        m_mesh = nullptr;
    }

    if(m_halfEdge != nullptr)
    {
        delete m_halfEdge->getNext()->getNext();
        delete m_halfEdge->getNext();
        delete m_halfEdge;

        m_halfEdge = nullptr;
    }
}


MeshHalfEdge* MeshTriangle::getHalfEdge()
{
    return m_halfEdge;
}

MeshVertex* MeshTriangle::getVertex0()
{
    return m_halfEdge->getOrigin();
}

MeshVertex* MeshTriangle::getVertex1()
{
    return m_halfEdge->getNext()->getOrigin();
}

MeshVertex* MeshTriangle::getVertex2()
{
    return m_halfEdge->getNext()->getNext()->getOrigin();
}