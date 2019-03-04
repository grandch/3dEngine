#include "MeshHalfEdge.h"

#include "MeshTriangle.h"
#include "MeshEdge.h"
#include "MeshVertex.h"
#include "Mesh.h"

MeshHalfEdge::MeshHalfEdge(Mesh* mesh, MeshVertex* origin, MeshVertex* target, MeshTriangle* triangle): m_mesh(mesh), m_origin(origin), m_triangle(triangle), m_edge(nullptr), m_next(nullptr), m_sibling(nullptr), m_opposite(nullptr)
{
    if(mesh != nullptr)
    {
        mesh->pushHalfEdge(this);
    }

    //search the opposite halfEdge
    MeshHalfEdge* other = target->getHalfEdgeTo(origin);

    if(other != nullptr)
    {
        if(other->getOpposite() != nullptr)
        {
            cout << "None manifold mesh" << endl;
        }

        //this is  opposite to m_opposite and vice versa
        m_opposite = other;
        other->m_opposite = this;

        //both have the same edge
        m_edge = m_opposite->m_edge;
    }

    if(m_edge == nullptr)
    {
        m_edge = new MeshEdge(mesh, origin, target);
    }

    origin->linkSibling(this);
}

MeshHalfEdge::~MeshHalfEdge()
{
    if(m_opposite != nullptr)
    {
        m_opposite->m_opposite = nullptr;
        m_opposite = nullptr;
    }
    else
    {
        delete m_edge;
    }

    m_edge = nullptr;
    m_next = nullptr;

    m_origin->unlinkSibling(this);

    if(m_mesh != nullptr)
    {
        m_mesh->popHalfEdge(this);
    }   
}


MeshVertex* MeshHalfEdge::getOrigin()
{
    return m_origin;
}

MeshVertex* MeshHalfEdge::getTarget()
{
    if(m_next == nullptr)
    {
        return nullptr;
    }
    return m_next->getOrigin();
}

MeshTriangle* MeshHalfEdge::getTriangle()
{
    return m_triangle;
}

MeshHalfEdge* MeshHalfEdge::getNext()
{
    return m_next;
}

void MeshHalfEdge::setNext(MeshHalfEdge* halfEdge)
{
    m_next = halfEdge;
}

MeshHalfEdge* MeshHalfEdge::getOpposite()
{
    return m_opposite;
}

MeshHalfEdge* MeshHalfEdge::getSibling()
{
    return m_sibling;
}

void MeshHalfEdge::setSibling(MeshHalfEdge* halfEdge)
{
    m_sibling = halfEdge;
}