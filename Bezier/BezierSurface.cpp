#include "BezierSurface.h"
#include "BezierCurve.h"
#include "../Mesh/MeshVertex.h"
#include "../Mesh/MeshEdge.h"

#include <vector>

BezierSurface::BezierSurface(BezierCurve* bc0, BezierCurve* bc1, BezierCurve* bc2, BezierCurve* bc3): m_mesh(nullptr)
{
    m_bcT.push_back(bc0);
    m_bcT.push_back(bc1);
    m_bcT.push_back(bc2);
    m_bcT.push_back(bc3);
}

BezierSurface::~BezierSurface()
{}

void BezierSurface::compute(int s, int t)
{
    if(m_mesh != nullptr)
    {
        free(m_mesh);
    }

    m_mesh = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");

    vector<vector<vec3>> pt;

    for(BezierCurve* bezierc : m_bcT)
    {
        pt.push_back(bezierc->compute(t));
    }

    vector<vector<vec3>> ps;

    for(int i = 0; i < t; i++)
    {
        BezierCurve* bcs = new BezierCurve(pt[0][i], pt[1][i], pt[2][i], pt[3][i]);
        ps.push_back(bcs->compute(s));   
    }

    for(int i = 0; i < t-1; i++)
    {
        for(int j = 0; j < s-1; j++)
        {
            vector<MeshVertex*> vertex;
            MeshVertex* v0 = new MeshVertex(m_mesh, std::to_string(i)+std::to_string(j));
            v0->setCoord(ps[i][j]);
            v0->setColor(vec3(1));
            vertex.push_back(v0);

            MeshVertex* v1 = new MeshVertex(m_mesh, std::to_string(i+1)+std::to_string(j));
            v1->setCoord(ps[i+1][j]);
            v1->setColor(vec3(1));
            vertex.push_back(v1);
            
            MeshVertex* v2 = new MeshVertex(m_mesh, std::to_string(i+1)+std::to_string(j+1));
            v2->setCoord(ps[i+1][j+1]);
            v2->setColor(vec3(1));
            vertex.push_back(v2);
            
            MeshVertex* v3 = new MeshVertex(m_mesh, std::to_string(i)+std::to_string(j+1));
            v3->setCoord(ps[i][j+1]);
            v3->setColor(vec3(1));
            vertex.push_back(v3);

            m_mesh->addPolygon(vertex);
        }
    }

    m_mesh->loadMesh();
}

void BezierSurface::draw(mat4 &projection, mat4 &modelview)
{
    m_mesh->draw(projection, modelview);
}