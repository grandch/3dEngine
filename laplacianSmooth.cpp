#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#include <vector>

void laplacianSmooth(Mesh* mesh, float alpha, float lambda)
{
    vector<MeshVertex*> vertices = mesh->getVertices();
    vector<vec3> laplacians;

    for(MeshVertex* v: vertices)
    {
        vector<MeshVertex*> neighbors = v->getVerticesAround();

        vec4 sum = vec4(0);
        for(MeshVertex* n: neighbors)
        {
            sum += n->getAttribute(0) - (1 - alpha) * v->getAttribute(0);
        }

       laplacians.push_back((alpha/neighbors.size()) * vec3(sum.x, sum.y, sum.z) );
    }

    for(int i = 0; i < laplacians.size(); i++)
    {
        mat4 transform = translate(laplacians[i]);
        vertices[i]->setCoord(lambda * transform * vertices[i]->getAttribute(0));
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
    importer.loadObjFile("Models/laplacianCurved.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(0,1,0.3), 1, 128);
    meshManager->addMesh("laplacianMesh", mesh);
    mesh->setDrawEdges(true);
    
    for(int i = 0; i < 2; i++)
    {
        laplacianSmooth(mesh, 0.99, 0.2);
    }

    mesh->loadMesh();

    scene.mainLoop();

    return 0;
}