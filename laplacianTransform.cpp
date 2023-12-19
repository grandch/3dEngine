#include "Scene/Scene.h"
#include "Mesh/MeshVertex.h"
#include "Mesh/MeshTriangle.h"

#include <vector>

void vertexLaplacian(MeshVertex* v, float alpha)
{
    vector<MeshVertex*> neighbors = v->getVerticesAround();

    float sum = 0;
    for(MeshVertex* neighbor: neighbors)
    {
        sum += neighbor->getLaplacian() - (1 - alpha) * v->getLaplacian();
    }

    v->setLaplacian(alpha * sum / neighbors.size());
}

void laplacianIt(Mesh* mesh, MeshVertex* heatVertex, float alpha)
{
    vector<MeshVertex*> vertices = mesh->getVertices();

    for(MeshVertex* v: vertices)
    {
        vertexLaplacian(v, alpha);
    }

    heatVertex->setLaplacian(1.0f);
    mesh->applyLaplacian();
}

float transfertFunction(float x)
{
    if(x < 0.4)
        return 0.f;
    return 1.f;
}

void transformLaplacian(Mesh* mesh, vec3 tr)
{
    vector<MeshVertex*> vertices = mesh->getVertices();

    for(MeshVertex* v: vertices)
    {
        mat4 transform = translate(transfertFunction(v->getLaplacian()) * tr);
        v->setCoord(transform * v->getAttribute(0));
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

    MeshVertex* heatVertex = mesh->getVertex("v(8, 8, 0)");
    mesh->initLaplacian(heatVertex);

    for(int i = 0; i < 500; i++)
        laplacianIt(mesh, heatVertex, 0.99);

    transformLaplacian(mesh, vec3(0, 0.5, 1));

    mesh->loadMesh();

    scene.mainLoop();

    return 0;
}