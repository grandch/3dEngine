#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#include <vector>
#include <set>

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

set<MeshVertex*> getNextRing(set<MeshVertex*>* actualRing, set<MeshVertex*>* previousRing)
{
    set<MeshVertex*>* nextRing = new set<MeshVertex*>;

    for(MeshVertex* actualV: actualRing)
    {
        vector<MeshVertex*> vArround = getVerticesArround(actualV);
        
        for(MeshVertex* arroundV: vArround)
        {
            if(!actualRing->contains(arroundV) && !previousRing->contains(arroundV))
            {
                nextRing->insert(arroundV);
            }
        }
    }

    return nextRing;
}

void laplacianIt(Mesh* mesh, MeshVertex* heatVertex, int it)
{
    // init
    heatVertex->setColor(vec3(1,1,1));
    vector<set<MeshVertex*>*> rings;
    set<MeshVertex*>* heat; heat->insert(heatVertex);
    set<MeshVertex*>* base;

    rings.push_back(base);
    rings.push_back(heat);

    for(int i = 2; i < it+2; i++)
    {
        rings.push_back(getNextRing(rings[i-1], rings[i-2]));
    }

    // compute laplacian with rings
}

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;

    MeshManager* meshManager = scene.getMeshManager();
    LightManager* lightManager = scene.getLightManager();
}