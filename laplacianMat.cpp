#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include <Eigen/Dense>

#include <vector>
#include <map>

vector<MeshVertex*> getNeighbors(MeshVertex* v)
{
    vector<MeshVertex*> vertices;
    vector<MeshTriangle*> triangles = v->getTrianglesAround();

    // id verification
    for(int i = 0; i < triangles.size(); i++)
    {
        if(!triangles[i]->getVertex0()->getNumber() != v->getNumber())
        {
            vertices.push_back(triangles[i]->getVertex0());
        }

        if(!triangles[i]->getVertex1()->getNumber() != v->getNumber())
        {
            vertices.push_back(triangles[i]->getVertex1());
        }

        if(!triangles[i]->getVertex2()->getNumber() != v->getNumber())
        {
            vertices.push_back(triangles[i]->getVertex2());
        }
    }

    return vertices;
}

bool mapContains(map<int, MeshVertex*> map, int key)
{
    auto search = map.find(key);
    return search != map.end();
}

map<int, MeshVertex*> getNextRing(map<int, MeshVertex*> actualRing, map<int, MeshVertex*> previousRing)
{
    map<int, MeshVertex*> nextRing;

    for(std::pair<int, MeshVertex*> actualV: actualRing)
    {
        vector<MeshVertex*> vArround = getNeighbors(actualV.second);
        
        for(MeshVertex* arroundV: vArround)
        {
            if(!mapContains(actualRing, arroundV->getNumber()) && !mapContains(previousRing, arroundV->getNumber()))
            {
                nextRing.insert({arroundV->getNumber(), arroundV});
            }
        }
    }

    return nextRing;
}

void vertexLaplacian(MeshVertex* v, float alpha)
{
    vector<MeshVertex*> neighbors = getNeighbors(v);

    float sum = 0;
    for(MeshVertex* neighbor: neighbors)
    {
        sum += neighbor->getLaplacian() + (1 - alpha) * v->getLaplacian();
    }

    v->setLaplacian(alpha * sum / neighbors.size());
}

void ringLaplacian(map<int, MeshVertex*> ring, float alpha)
{
    for(std::pair<int, MeshVertex*> v: ring)
    {
        vertexLaplacian(v.second, alpha);
    }
}

void applyLaplacian(vector<map<int, MeshVertex*>> rings)
{
    for(map<int, MeshVertex*> ring: rings)
    {
        for(std::pair<int, MeshVertex*> v: ring)
        {
            v.second->applyLaplacian();
        }
    }
}

bool isBoundarie(MeshVertex* v)
{
    return getNeighbors(v).size() < 6;
}

bool areNeighbors(MeshVertex* v1, MeshVertex* v2)
{
    vector<MeshVertex*> neighbors = getNeighbors(v1);
    for(MeshVertex* n: neighbors)
    {
        if(n->getName() == v2->getName())
        {
            return true;
        }
    }

    return false;
}

std::pair<MeshVertex*, MeshVertex*> getSharedNeighbors(MeshVertex* v1, MeshVertex* v2)
{
    bool foundFirstOne = false;
    std::pair<MeshVertex*, MeshVertex*> tr;
    vector<MeshVertex*> neighbors1 = getNeighbors(v1);
    vector<MeshVertex*> neighbors2 = getNeighbors(v2);

    for(MeshVertex* n1: neighbors1)
    {
        for(MeshVertex* n2: neighbors2)
        {
            if(n1->getName() == n2->getName())
            {
                if(!foundFirstOne)
                {
                    tr.first = n1;
                    foundFirstOne = true;
                }
                else
                {
                    tr.second = n1;
                    return tr;
                }
            }
        }
    }

    return {nullptr, nullptr};
}

void laplacian(Mesh* mesh, map<int, MeshVertex*> heatPoints, map<int, MeshVertex*> boundaries)
{
    // init rings
    vector<MeshVertex*> vertices;

    map<int, MeshVertex*> ring = heatPoints;
    map<int, MeshVertex*> previousRing = {};
    map<int, MeshVertex*> nextRing;

    int nbHeatPoints, nbBoundaries, nVertices, nbVertices;

    nbHeatPoints = heatPoints.size();
    for(std::pair<int, MeshVertex*> v: heatPoints)
    {
        vertices.push_back(v.second);
    }
    
    nbVertices = 0;
    while (ring.size() > 0)
    {
        nextRing = getNextRing(ring, previousRing);

        // remove all boundaries and heatpoints for generating new ring
        for(auto v = nextRing.begin(); v != nextRing.end();)
        {
            if(isBoundarie(v->second) || mapContains(boundaries, v->first) || mapContains(heatPoints, v->first))
                v = nextRing.erase(v);
            else
            {
                nbVertices++;
                vertices.push_back(v->second);
                // std::cout << v->second->getNumber() << endl;
                v++;
            }
        }

        previousRing = ring;
        ring = nextRing;
    }

    nbBoundaries = boundaries.size();
    for(std::pair<int, MeshVertex*> v: boundaries)
    {
        vertices.push_back(v.second);
    }
    
    // init matrices
    nVertices = vertices.size();
    Eigen::MatrixXf vertexFactors(nVertices,nVertices);
    Eigen::VectorXf values(nVertices);

    std::cout << "nHeat : " << nbHeatPoints << ", nBound : " << nbBoundaries << ", nbVertices : " << nbVertices << ", nTotalVertices : " << nVertices << endl;


    for(int i = 0; i < nbHeatPoints; i++)
    {
        if(!mapContains(heatPoints, vertices[i]->getNumber()))
            std::cout << "False" << endl;
        else
        {}
    }
    for(int i = nbHeatPoints; i < nbHeatPoints + nbVertices; i++)
    {
        if(mapContains(heatPoints, vertices[i]->getNumber()) || mapContains(boundaries, vertices[i]->getNumber()))
            std::cout << "False" << endl;
        else
        {}
    }
    for(int i = nbHeatPoints + nbVertices; i < nbHeatPoints + nbVertices + nbBoundaries; i++)
    {
        if(!mapContains(boundaries, vertices[i]->getNumber()))
            std::cout << "False" << endl;
        else
        {}
    }

    // construct matrices
    for(int i = 0; i < nVertices; i++)
    {
        if(mapContains(heatPoints, vertices[i]->getNumber()))
        {
            values[i] = -1.f;
        }
        else
            values[i] = 0.f;

        for (int j = 0; j < i; j++)
            vertexFactors(i,j) = 0.f;

        for (int j = i; j < nVertices; j++)
        {
            if(isBoundarie(vertices[i]) || mapContains(boundaries, vertices[i]->getNumber()))
            {
                // 1 * /\ v + 0 * /\ n = 0
                if(i == j)
                    vertexFactors(i,j) = 1.f;
                else
                    vertexFactors(i,j) = 0.f;
            }
            else
            {
                // (valance aux angles) * /\ n - /\ v = value
                if(i == j)
                    vertexFactors(i,j) = -1.f;
                else
                {
                    if(areNeighbors(vertices[i], vertices[j]))
                    {
                        std::pair<MeshVertex*, MeshVertex*> sharedNeighbors = getSharedNeighbors(vertices[i], vertices[j]);
                        vec4 ca = vertices[i]->getAttribute(0) - sharedNeighbors.first->getAttribute(0);
                        vec4 cb = vertices[j]->getAttribute(0) - sharedNeighbors.first->getAttribute(0);
                        vec4 da = vertices[i]->getAttribute(0) - sharedNeighbors.second->getAttribute(0);
                        vec4 db = vertices[j]->getAttribute(0) - sharedNeighbors.second->getAttribute(0);

                        vertexFactors(i,j) = (float) ((dot(ca, cb) + dot(da, db)));
                    }
                    else
                        vertexFactors(i,j) = 0.f;
                }
            }
        }
    }

    
    // compute X matrix
    // LU decomposition of a
    Eigen::FullPivLU<Eigen::MatrixXf> lu(vertexFactors);

    // solve linear system ax = b
    Eigen::VectorXf x = lu.solve(values);
    std::cout << x << endl;

    // set laplacian value to vertices
    for(int i = 0; i < nVertices; i++)
    {
        vertices[i]->setLaplacian(x[i]);
        vertices[i]->applyLaplacian();
    }
}

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;

    MeshManager* meshManager = scene.getMeshManager();

    Mesh* mesh = new Mesh("Shaders/Color.vert", "Shaders/Color.frag");
    Importer importer(mesh);
    importer.loadObjFile("Models/laplacianMeshIt.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(0,1,0.3), 1, 128);
    meshManager->addMesh("laplacianMesh", mesh);
    mesh->setDrawEdges(true);

    // select boundaries vertices
    map<int, MeshVertex*> boundaries;
    float coords[] = {-0.5,0.5,0,
                    -0.5,0.75,0,
                    -0.25,0.75,0,
                    0,0.5,0,
                    0.25,0.5,0,
                    0.5,0.5,0,
                    0.75,0.5,0,
                    0.75,0.25,0,
                    1,0,0,
                    0.75,0,0,
                    0.5,0,0,
                    0.25,0,0,
                    0.25,-0.25,0,
                    0,0.25,0,
                    -0.25,0,0,
                    -0.5,0.25,0,
                    -0.75,0.5,0};
    
    for(int i = 0; i < 17*3; i+=3)
    {
        vec3 coord(coords[i], coords[i+1], coords[i+2]);
        vector<MeshVertex*> vertices = mesh->getVertex(coord);
        boundaries.insert({vertices[0]->getNumber(), vertices[0]});
    }

    // select heat vertices
    map<int, MeshVertex*> heatPoints;
    float coords2[] = {0,0,0,
              -0.25,0.25,0};
    
    for(int i = 0; i < 3*2; i+=3)
    {
        vec3 coord(coords2[i], coords2[i+1], coords2[i+2]);
        vector<MeshVertex*> vertices = mesh->getVertex(coord);
        heatPoints.insert({vertices[0]->getNumber(), vertices[0]});
    }

    // compute laplacian
    laplacian(mesh, heatPoints, boundaries);

    mesh->loadMesh();

    scene.mainLoop();

    return 0;
}