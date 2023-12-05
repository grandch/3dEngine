#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

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
    vector<map<int, MeshVertex*>> rings;
    vector<MeshVertex*> vertices;

    map<int, MeshVertex*> ring = heatPoints;
    map<int, MeshVertex*> previousRing = {};
    map<int, MeshVertex*> nextRing;

    while (ring.size() > 0)
    {
        nextRing = getNextRing(ring, previousRing);
        rings.push_back(nextRing);

        // remove all boundaries for generating new ring
        for(auto v = nextRing.begin(); v != nextRing.end();)
        {
            vertices.push_back(v->second);
            if(isBoundarie(v->second) || mapContains(boundaries, v->first))
                v = nextRing.erase(v);
            else
                v++;
        }

        previousRing = ring;
        ring = nextRing;
    }
    
    // init matrix
    int nVertices = vertices.size();
    float vertexFactors[nVertices][nVertices];

    // construct weight matrix
    for(int i = 0; i < nVertices; i++)
    {
        for (int j = i; j < nVertices; j++)
        {
            // TODO check if fill the matrix symetricaly or fill the second part with zeros
            if(isBoundarie(vertices[i]) || mapContains(boundaries, vertices[i]->getNumber()))
            {
                if(i == j)
                    vertexFactors[i][j] = 1;
                else
                {
                    vertexFactors[i][j] = 0;
                    vertexFactors[j][i] = 0;
                }
            }
            else
            {
                if(i == j)
                    vertexFactors[i][j] = -1;
                else
                {
                    if(areNeighbors(vertices[i], vertices[j]))
                    {
                        std::pair<MeshVertex*, MeshVertex*> sharedNeighbors = getSharedNeighbors(vertices[i], vertices[j]);
                        vec4 ca = vertices[i]->getAttribute(0) - sharedNeighbors.first->getAttribute(0);
                        vec4 cb = vertices[j]->getAttribute(0) - sharedNeighbors.first->getAttribute(0);
                        vec4 da = vertices[i]->getAttribute(0) - sharedNeighbors.second->getAttribute(0);
                        vec4 db = vertices[j]->getAttribute(0) - sharedNeighbors.second->getAttribute(0);

                        vertexFactors[i][j] = dot(ca, cb) + dot(da, db);
                        vertexFactors[j][i] = dot(ca, cb) + dot(da, db);
                    }
                    else
                    {
                        vertexFactors[i][j] = 0;
                        vertexFactors[j][i] = 0;
                    }
                }
            }
        }
    }

    // init laplacian matrix

    // init values matrix

    // compute matrix

}

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;

    MeshManager* meshManager = scene.getMeshManager();

    Mesh* mesh = new Mesh("Shaders/Color.vert", "Shaders/Color.frag");
    Importer importer(mesh);
    importer.loadObjFile("Models/laplacianMesh.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(0,1,0.3), 1, 128);
    meshManager->addMesh("laplacianMesh", mesh);
    mesh->setDrawEdges(true);

    MeshVertex* heatVertex = mesh->getVertex("v(8, 8, 0)");

    // laplacian(mesh, heatVertex, 16);

    mesh->loadMesh();

    scene.mainLoop();

    return 0;
}