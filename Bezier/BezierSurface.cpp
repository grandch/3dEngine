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

    m_mesh = new Mesh("Shaders/BRDFDiffuse.vert", "Shaders/BRDFDiffuse.frag");

    vector<vector<vec3>> pt;

    for(BezierCurve* bezierc : m_bcT)
    {
        pt.push_back(bezierc->compute(t));
    }

    vector<vector<vec3>> ps;
    vector<vector<MeshVertex*>> vs;

    for(int i = 0; i < t; i++)
    {
        BezierCurve* bcs = new BezierCurve(pt[0][i], pt[1][i], pt[2][i], pt[3][i]);
        //ps.push_back(bcs->compute(s));

        vector<vec3> p = bcs->compute(s);
        vector<MeshVertex*> v;

        for(vec3 pp: p)
        {
            MeshVertex* vv = new MeshVertex(m_mesh, "v");
            vv->setCoord(pp);
            vv->setColor(vec3(1));
            v.push_back(vv);
        }

        vs.push_back(v);
    }

    for(int i = 0; i < t-1; i++)
    {
        for(int j = 0; j < s-1; j++)
        {
            vector<MeshVertex*> vertex;
            vertex.push_back(vs[i][j]);
            vertex.push_back(vs[i+1][j]);
            vertex.push_back(vs[i+1][j+1]);
            vertex.push_back(vs[i][j+1]);
            
            m_mesh->addPolygon(vertex);
        }
    }

    cout << "load mesh" << endl;
    m_mesh->loadMesh();
    cout << "end load mesh" << endl;
}

void BezierSurface::draw(mat4 &projection, mat4 &modelview)
{
    m_mesh->draw(projection, modelview);
}

Mesh* BezierSurface::getMesh()
{
    return m_mesh;
}