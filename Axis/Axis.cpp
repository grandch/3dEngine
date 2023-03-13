#include "Axis.h"
#include "../Mesh/MeshVertex.h"
#include "../Mesh/MeshEdge.h"


Axis::Axis(): m_meshX(nullptr), m_meshY(nullptr), m_meshZ(nullptr)
{}

Axis::~Axis()
{
    free(m_meshX);
    free(m_meshY);
    free(m_meshZ);
}

void Axis::loadAxis()
{
    if(m_meshX != nullptr){free(m_meshX);}
    if(m_meshY != nullptr){free(m_meshY);}
    if(m_meshZ != nullptr){free(m_meshZ);}

    m_meshX = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    m_meshY = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    m_meshZ = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");

    MeshVertex* vX1 = new MeshVertex(m_meshX, "vx1");
    vX1->setCoord(vec3(-10, 0, 0));
    MeshVertex* vX2 = new MeshVertex(m_meshX, "vx2");
    vX1->setCoord(vec3(10, 0, 0));

    MeshVertex* vY1 = new MeshVertex(m_meshY, "vy1");
    vY1->setCoord(vec3(0, -10, 0));
    MeshVertex* vY2 = new MeshVertex(m_meshY, "vy2");
    vY1->setCoord(vec3(0, 10, 0));

    MeshVertex* vZ1 = new MeshVertex(m_meshZ, "vz1");
    vZ1->setCoord(vec3(0, 0, -10));
    MeshVertex* vZ2 = new MeshVertex(m_meshZ, "vz2");
    vZ1->setCoord(vec3(0, 0, 10));

    MeshEdge* eX = new MeshEdge(m_meshX, vX1, vX2);
    MeshEdge* eY = new MeshEdge(m_meshY, vY1, vY2);
    MeshEdge* eZ = new MeshEdge(m_meshZ, vZ1, vZ2);

    m_meshX->loadMesh();
    m_meshY->loadMesh();
    m_meshZ->loadMesh();
}

void Axis::draw(mat4 &projection, mat4 &modelview)
{
    m_meshX->draw(projection, modelview);
    m_meshY->draw(projection, modelview);
    m_meshZ->draw(projection, modelview);
}