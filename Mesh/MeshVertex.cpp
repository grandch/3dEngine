#include "MeshVertex.h"

#include "MeshHalfEdge.h"
#include "MeshTriangle.h"
#include "Mesh.h"

MeshVertex::MeshVertex(Mesh* mesh, string name): m_name(name), m_mesh(mesh), m_halfEdge(nullptr), m_uv(vec2(0)), m_curentLaplacian(0), m_computedLaplacian(0)
{
    if(mesh != nullptr)
    {
        mesh->pushVertex(this);
    }
            
    m_attributes.push_back(vec4(0.0)); //coords
    m_attributes.push_back(vec4(0.0)); //color
    m_attributes.push_back(vec4(0.0)); //normal
}

MeshVertex::~MeshVertex()
{
    if(m_mesh != nullptr)
    {
        m_mesh->popVertex(this);
        m_mesh = nullptr;
    }

    //remove triangles around the vertex
    MeshHalfEdge* halfEdge = m_halfEdge;
    while(halfEdge != nullptr)
    {
        MeshHalfEdge* next = halfEdge->getSibling();
        delete halfEdge->getTriangle();
        halfEdge = next;
    }
}

vector<MeshTriangle*> MeshVertex::getTrianglesAround()
{
    MeshHalfEdge* halfEdge = m_halfEdge; //starting half edge

    vector<MeshTriangle*> triangles;
    MeshTriangle* triangle;

    while(halfEdge != nullptr)
    {
        triangle = halfEdge->getTriangle();

        triangles.push_back(triangle);

        halfEdge = halfEdge->getSibling();
    }

    return triangles;
}

vector<MeshVertex*> MeshVertex::getVerticesAround()
{
    MeshHalfEdge* halfEdge = m_halfEdge; //starting half edge

    vector<MeshVertex*> vertices;
    MeshTriangle* triangle;

    while(halfEdge != nullptr)
    {
        triangle = halfEdge->getTriangle();

        vertices.push_back(triangle->getVertex0());
        vertices.push_back(triangle->getVertex1());
        vertices.push_back(triangle->getVertex2());

        halfEdge = halfEdge->getSibling();
    }

    return vertices;
}

int MeshVertex::getNumber()
{
    return m_number;
}

void MeshVertex::setNumber(int number)
{
    m_number = number;
}

string MeshVertex::getName()
{
    return m_name;
}

vec4 MeshVertex::getAttribute(int index)
{
    return m_attributes[index];
}

vec2 MeshVertex::getUv()
{
    return m_uv;
}

MeshVertex* MeshVertex::setCoord(vec3 coord) //p151
{
    m_attributes[0] = vec4(coord.x, coord.y, coord.z, 1.0f);
    return this;
}

MeshVertex* MeshVertex::setColor(vec3 color)
{
    m_attributes[1] = vec4(color.x, color.y, color.z, 1.0f);
    return this;
}

MeshVertex* MeshVertex::setNormal(vec3 normal)
{
    m_attributes[2] = vec4(normal.x, normal.y, normal.z, 1.0f);
    return this;
}

MeshVertex *MeshVertex::setUv(vec2 uv)
{
    m_uv = uv;
    return this;
}

void MeshVertex::computeNormal()
{
    vector<MeshTriangle*> faces = getTrianglesAround();
    
    vec3 normal = vec3(0);
    for(MeshTriangle* face: faces)
    {
        normal += face->getNormal();
    }
    
    setNormal(normalize(normal));
}

MeshHalfEdge* MeshVertex::getHalfEdgeTo(MeshVertex* vertex)
{
    MeshHalfEdge* halfEdge = m_halfEdge;

    while(halfEdge != nullptr)
    {
        if(halfEdge->getTarget() == vertex)
        {
            return halfEdge;
        }

        halfEdge = halfEdge->getSibling();
    }
    return nullptr;
}

void MeshVertex::linkSibling(MeshHalfEdge* halfEdge)
{
    halfEdge->setSibling(m_halfEdge);
    m_halfEdge = halfEdge;
}

void MeshVertex::unlinkSibling(MeshHalfEdge* halfEdge)
{
    if(m_halfEdge == halfEdge)
    {
        m_halfEdge = m_halfEdge->getSibling();
    }
    else
    {
        MeshHalfEdge* other = m_halfEdge;
        while(other != nullptr)
        {
            if(other->getSibling() == halfEdge)
            {
                other->setSibling(halfEdge->getSibling());
                break;
            }
            other = other->getSibling();
        }
    }
}

float MeshVertex::getLaplacian()
{
    return m_curentLaplacian;
}

void MeshVertex::setLaplacian(float laplacian)
{
    m_computedLaplacian = laplacian;
}

void MeshVertex::applyLaplacian()
{
    m_curentLaplacian = m_computedLaplacian;

    if(m_computedLaplacian < 0.25)
    {
        float coef = (m_computedLaplacian) / (0.25);
        m_attributes[1] = vec4(0., 0., 1. * coef, 1.);
    }
    else if (m_computedLaplacian < 0.5)
    {
        float coef = (m_computedLaplacian - 0.25) / (0.25);
        m_attributes[1] = vec4(0., 1. * coef, 0., 1.) + vec4(0., 0., 1. * (1-coef), 1.);
    }
    else if (m_computedLaplacian < 0.75)
    {
        float coef = (m_computedLaplacian - 0.5) / (0.25);
        m_attributes[1] = vec4(1. * coef, 0., 0., 1.) + vec4(0., 1. * (1-coef), 0., 1.);
    }
    else
    {
        float coef = (m_computedLaplacian - 0.75) / (0.25);
        m_attributes[1] = vec4(1. * coef, 1. * coef, 1. * coef, 1.) + vec4(1. * (1-coef), 0., 0., 1.);
    }
}
