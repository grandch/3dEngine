#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

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

void laplacian(Mesh* mesh, map<int, MeshVertex*> heatPoints, vector<MeshVertex> boundaries)
{
    // init matrix
    int nVertices = mesh->getVertices().size();

    // add heatpoints to matrix
        // check if the vertex is on a boundarie
            // apply the rule 

    map<int, MeshVertex*> ring = getNextRing(heatPoints, {});
    map<int, MeshVertex*> previousRing = heatPoints;
    map<int, MeshVertex*> nextRing;

    int i = heatPoints.size();
    while (i < nVertices)
    {
        i += ring.size();
        // add neighbors to matrix
            // check if the vertex is on a boundarie
                // apply the rule 
        
        nextRing = getNextRing(ring, previousRing);
        previousRing = ring;
        ring = nextRing;
    }
    

    // init weights matrix

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