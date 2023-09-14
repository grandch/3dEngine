#include "BezierSurface.h"
#include "BezierCurve.h"
#include "../Mesh/MeshVertex.h"
#include "../Mesh/MeshEdge.h"
#include "../Light/LightManager.h"

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

void BezierSurface::compute(int s, int t, string vertexShader, string fragmentShader)
{
    if(m_mesh != nullptr)
    {
        free(m_mesh);
    }

    m_mesh = new Mesh(vertexShader, fragmentShader);

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

            vs[i][j]->setUv(vec2(((float)i)/t, ((float)j)/s));
            vertex.push_back(vs[i][j]);

            vs[i+1][j]->setUv(vec2(((float)i+1)/t, ((float)j)/s));
            vertex.push_back(vs[i+1][j]);

            vs[i+1][j+1]->setUv(vec2(((float)i+1)/t, ((float)j+1)/s));
            vertex.push_back(vs[i+1][j+1]);

            vs[i][j+1]->setUv(vec2(((float)i)/t, ((float)j+1)/s));
            vertex.push_back(vs[i][j+1]);
            
            m_mesh->addPolygon(vertex);
        }
    }

    m_mesh->loadMesh();
}

void BezierSurface::draw(mat4 &projection, mat4 &view)
{
    m_mesh->draw(projection, view, new LightManager());

    for(BezierCurve* bc : m_bcT)
    {
        bc->draw(projection, view);
    }
}

void BezierSurface::transform(mat4 transf)
{
    m_mesh->translate(transf);

    for(BezierCurve* bc : m_bcT)
    {
        bc->transform(transf);
    }
}

Mesh* BezierSurface::getMesh()
{
    return m_mesh;
}