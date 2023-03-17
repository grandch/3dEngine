#include "Bezier.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshEdge.h"
#include "../Mesh/MeshVertex.h"

Bezier::Bezier(vec3 p1, vec3 p2, vec3 pc1, vec3 pc2): m_p1(p1), m_p2(p2), m_pc1(pc1), m_pc2(pc2), m_meshCurve(nullptr), m_meshControlPolygon(nullptr)
{
    m_meshControlPolygon = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");

    MeshVertex* v1 = new MeshVertex(m_meshControlPolygon, "V1");
    v1->setCoord(p1);
    v1->setColor(vec3(1));
    MeshVertex* v2 = new MeshVertex(m_meshControlPolygon, "V2");
    v2->setCoord(p2);
    v2->setColor(vec3(1));
    MeshVertex* vc1 = new MeshVertex(m_meshControlPolygon, "VC1");
    vc1->setCoord(pc1);
    vc1->setColor(vec3(1));
    MeshVertex* vc2 = new MeshVertex(m_meshControlPolygon, "VC2");
    vc2->setCoord(pc2);
    vc2->setColor(vec3(1));

    MeshEdge* e0 = new MeshEdge(m_meshControlPolygon, v1, vc1);
    MeshEdge* e1 = new MeshEdge(m_meshControlPolygon, vc1, vc2);
    MeshEdge* e2 = new MeshEdge(m_meshControlPolygon, vc2, v2);

    m_meshControlPolygon->loadMesh();
}

Bezier::~Bezier()
{}

vec3 Bezier::b(float t)
{
    float x = m_p1[0] * pow(1-t, 3) + 3 * m_pc1[0] * t * pow(1-t, 3) + 3 * m_pc2[0] * pow(t,2) * (1-t) + m_p2[0] * pow(t,3);
    float y = m_p1[1] * pow(1-t, 3) + 3 * m_pc1[1] * t * pow(1-t, 3) + 3 * m_pc2[1] * pow(t,2) * (1-t) + m_p2[1] * pow(t,3);
    float z = m_p1[2] * pow(1-t, 3) + 3 * m_pc1[2] * t * pow(1-t, 3) + 3 * m_pc2[2] * pow(t,2) * (1-t) + m_p2[2] * pow(t,3);
    return vec3(x, y, z);
}

Mesh* Bezier::compute(int nbPoints)
{
    if(m_meshCurve != nullptr)
    {
        free(m_meshCurve);
    }

    m_meshCurve = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    
    float u = 1.0/nbPoints;
    
    MeshVertex* v1 = new MeshVertex(m_meshCurve, "0");
    v1->setCoord(b(0));
    MeshVertex* v2;

    for(int i = 1; i < nbPoints; i++)
    {
        v2 = new MeshVertex(m_meshCurve, std::to_string(i));
        v2->setCoord(b(u*i));
        new MeshEdge(m_meshCurve, v1, v2);
        v1 = v2;
    }

    v2 = new MeshVertex(m_meshCurve, std::to_string(nbPoints));
    v2->setCoord(b(1));
    new MeshEdge(m_meshCurve, v1, v2);


    m_meshCurve->loadMesh();

    return m_meshCurve;
}

void Bezier::draw(mat4 &projection, mat4 &modelview)
{
    m_meshCurve->draw(projection, modelview);
    m_meshControlPolygon->draw(projection, modelview);
}