#include "Bezier.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshEdge.h"
#include "../Mesh/MeshVertex.h"

Bezier::Bezier(vec3 p1, vec3 p2, vec3 pc): m_p1(p1), m_p2(p2), m_pc(pc), m_mesh(nullptr)
{}

Bezier::~Bezier()
{}

vec3 Bezier::b(float t)
{
    cout << t << endl;
    float x = pow(1-t, 2) * m_p1[0] + 2 * (1-t) * t * m_pc[0] + pow(t, 2) * m_p2[0];
    float y = pow(1-t, 2) * m_p1[1] + 2 * (1-t) * t * m_pc[1] + pow(t, 2) * m_p2[1];
    float z = pow(1-t, 2) * m_p1[2] + 2 * (1-t) * t * m_pc[2] + pow(t, 2) * m_p2[2];
    return vec3(x, y, z);
}

Mesh* Bezier::compute(int nbPoints)
{
    if(m_mesh != nullptr)
    {
        free(m_mesh);
    }

    m_mesh = new Mesh("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    
    float u = 1.0/nbPoints;
    
    MeshVertex* v1 = new MeshVertex(m_mesh, "0");
    v1->setCoord(b(0));
    MeshVertex* v2;

    for(int i = 1; i < nbPoints; i++)
    {
        v2 = new MeshVertex(m_mesh, std::to_string(i));

        v2->setCoord(b(u*i));

        new MeshEdge(m_mesh, v1, v2);
        v1 = v2;
    }

    m_mesh->loadMesh();

    return m_mesh;
}

void Bezier::draw(mat4 &projection, mat4 &modelview)
{
    m_mesh->draw(projection, modelview);
}