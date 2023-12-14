#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#include <vector>
#include <map>

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
        vector<MeshVertex*> vArround = actualV.second->getVerticesAround();
        
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
    vector<MeshVertex*> neighbors = v->getVerticesAround();

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

void laplacianIt(Mesh* mesh, MeshVertex* heatVertex, int it, float alpha)
{
    // init
    mesh->initLaplacian(heatVertex);
    vector<map<int, MeshVertex*>> rings;
    map<int, MeshVertex*> heat; heat.insert({heatVertex->getNumber(), heatVertex});
    map<int, MeshVertex*> base;

    rings.push_back(base);
    rings.push_back(heat);

    for(int i = 2; i < it+2; i++)
    {
        rings.push_back(getNextRing(rings[i-1], rings[i-2]));
    }

    // remove base and heat rings (no computing needed)
    rings.erase(rings.begin(), rings.begin()+2);

    // compute laplacian with rings
    for(int i = 0; i < it; i++)
    {
        for(int j = 0; j <= i && j < rings.size(); j++)
        {
            map<int, MeshVertex*> curentRing = rings[j];
            ringLaplacian(curentRing, alpha);
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

    Mesh* mesh = new Mesh("Shaders/Color.vert", "Shaders/Color.frag");
    Importer importer(mesh);
    importer.loadObjFile("Models/laplacianMesh.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(0,1,0.3), 1, 128);
    meshManager->addMesh("laplacianMesh", mesh);
    mesh->setDrawEdges(true);

    MeshVertex* heatVertex = mesh->getVertex("v(8, 8, 0)");

    laplacianIt(mesh, heatVertex, 32, 0.45);

    mesh->loadMesh();

    scene.mainLoop();

    return 0;
}