#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#include <vector>
#include <map>

vector<MeshVertex*> getVerticesArround(MeshVertex* v)
{
    vector<MeshVertex*> vertices;
    vector<MeshTriangle*> triangles = v->getTrianglesAround();

    // id verification
    for(int i = 0; i < 3; i++)
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
        vector<MeshVertex*> vArround = getVerticesArround(actualV.second);
        
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

void vertexLaplacian(MeshVertex* v)
{
    vector<MeshVertex*> neighbors = getVerticesArround(v);

    float sum = 0;
    for(MeshVertex* neighbor: neighbors)
    {
        sum += neighbor->getLaplacian() - v->getLaplacian();
    }

    v->setLaplacian(sum / neighbors.size());
}

void ringLaplacian(map<int, MeshVertex*> ring)
{
    for(std::pair<int, MeshVertex*> v: ring)
    {
        vertexLaplacian(v.second);
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

void laplacianIt(Mesh* mesh, MeshVertex* heatVertex, int it)
{
    // init
    heatVertex->setColor(vec3(1,1,1));
    vector<map<int, MeshVertex*>> rings;
    map<int, MeshVertex*> heat; heat.insert({heatVertex->getNumber(), heatVertex});
    map<int, MeshVertex*> base;

    rings.push_back(base);
    rings.push_back(heat);

    for(int i = 2; i < it+2; i++)
    {
        rings.push_back(getNextRing(rings[i-1], rings[i-2]));
    }

    // compute laplacian with rings
    for(int i = 0; i < it; i++)
    {
        for(int j = 0; j <= i; j++)
        {
            map<int, MeshVertex*> curentRing = rings[j];
            ringLaplacian(curentRing);
        }
        applyLaplacian(rings);
    }
}

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;

    MeshManager* meshManager = scene.getMeshManager();
    LightManager* lightManager = scene.getLightManager();
}