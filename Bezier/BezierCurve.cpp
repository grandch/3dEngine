#include "BezierCurve.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshEdge.h"
#include "../Mesh/MeshVertex.h"

BezierCurve::BezierCurve(vec3 p0, vec3 p1, vec3 p2, vec3 p3): m_p0(p0), m_p3(p3), m_p1(p1), m_p2(p2), m_meshCurve(nullptr), m_meshControlPolygon(nullptr), m_next(nullptr)
{
    m_meshControlPolygon = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");

    MeshVertex* v1 = new MeshVertex(m_meshControlPolygon, "V1");
    v1->setCoord(p0);
    v1->setColor(vec3(1));
    MeshVertex* v2 = new MeshVertex(m_meshControlPolygon, "V2");
    v2->setCoord(p3);
    v2->setColor(vec3(1));
    MeshVertex* vc1 = new MeshVertex(m_meshControlPolygon, "VC1");
    vc1->setCoord(p1);
    vc1->setColor(vec3(1));
    MeshVertex* vc2 = new MeshVertex(m_meshControlPolygon, "VC2");
    vc2->setCoord(p2);
    vc2->setColor(vec3(1));

    MeshEdge* e0 = new MeshEdge(m_meshControlPolygon, v1, vc1);
    MeshEdge* e1 = new MeshEdge(m_meshControlPolygon, vc1, vc2);
    MeshEdge* e2 = new MeshEdge(m_meshControlPolygon, vc2, v2);

    m_meshControlPolygon->loadMesh();
}

BezierCurve::~BezierCurve()
{}

vec3 BezierCurve::getPoint(int i)
{
    switch (i)
    {
    case 0:
        return m_p0;
    case 1:
        return m_p1;
    case 2:
        return m_p2;
    default:
        return m_p3;
    }
}

void BezierCurve::addSegment(vec3 p1, vec3 p2, vec3 p3)
{
    if(m_next != nullptr)
    {
        m_next->addSegment(p1, p2, p3);
    }
    else
    {
        m_next = new BezierCurve(m_p3, p1, p2, p3);
    }
}

vec3 BezierCurve::b(float t)
{
    float x = m_p0[0] * pow(1-t, 3) + 3 * m_p1[0] * t * pow(1-t, 3) + 3 * m_p2[0] * pow(t,2) * (1-t) + m_p3[0] * pow(t,3);
    float y = m_p0[1] * pow(1-t, 3) + 3 * m_p1[1] * t * pow(1-t, 3) + 3 * m_p2[1] * pow(t,2) * (1-t) + m_p3[1] * pow(t,3);
    float z = m_p0[2] * pow(1-t, 3) + 3 * m_p1[2] * t * pow(1-t, 3) + 3 * m_p2[2] * pow(t,2) * (1-t) + m_p3[2] * pow(t,3);
    return vec3(x, y, z);
}

vector<vec3> BezierCurve::compute(int nbPoints)
{
    if(m_meshCurve != nullptr)
    {
        free(m_meshCurve);
    }

    m_meshCurve = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    
    vector<vec3> p;
    p.push_back(b(0));

    float u = 1.0/nbPoints;
    
    MeshVertex* v1 = new MeshVertex(m_meshCurve, "0");
    v1->setCoord(b(0));
    MeshVertex* v2;

    for(int i = 1; i < nbPoints; i++)
    {
        v2 = new MeshVertex(m_meshCurve, std::to_string(i));
        v2->setCoord(b(u*i));
        p.push_back(b(u*i));
        new MeshEdge(m_meshCurve, v1, v2);
        v1 = v2;
    }

    v2 = new MeshVertex(m_meshCurve, std::to_string(nbPoints));
    v2->setCoord(b(1));
    p.push_back(b(1));
    new MeshEdge(m_meshCurve, v1, v2);


    m_meshCurve->loadMesh();

    if(m_next != nullptr)
    {
        vector<vec3> n = m_next->compute(nbPoints);
        p.insert(p.end(), n.begin(), n.end());
    }

    return p;
}

void BezierCurve::draw(mat4 &projection, mat4 &modelview)
{
    m_meshCurve->draw(projection, modelview);
    m_meshControlPolygon->draw(projection, modelview);

    if(m_next != nullptr)
    {
        m_next->draw(projection, modelview);
    }
}