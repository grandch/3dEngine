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

bool mapContains(map<int, std::pair<float, MeshVertex*>> map, int key)
{
    auto search = map.find(key);
    return search != map.end();
}

bool areNeighbors(MeshVertex* v1, MeshVertex* v2)
{
    MeshHalfEdge* he = v1->getHalfEdgeTo(v2);
    return he != nullptr;
}

std::pair<MeshVertex*, MeshVertex*> getSharedNeighbors(MeshVertex* v1, MeshVertex* v2)
{
    MeshHalfEdge* he = v1->getHalfEdgeTo(v2);
    MeshHalfEdge* opp = he->getOpposite();

    if(opp == nullptr)
        return {nullptr, nullptr};

    MeshHalfEdge* next1 = he->getNext();
    MeshHalfEdge* next2 = opp->getNext();

    return {next1->getTarget(), next2->getTarget()};
}

bool isBoundary(MeshVertex* v)
{
    MeshHalfEdge* he = v->getHalfEdge();

    while(he->getSibling() != nullptr)
    {
        if(he->getOpposite() == nullptr)
            return true;
        he = he->getSibling();
    }

    return false;
}

void laplacian(Mesh* mesh, map<int, std::pair<float, MeshVertex*>> heatPoints)
{
    vector<MeshVertex*> vertices = mesh->getVertices();
    int nVertices = vertices.size();

    // init matrices
    Eigen::MatrixXf vertexFactors(nVertices,nVertices);
    Eigen::VectorXf values(nVertices);

    for(int i = 0; i < nVertices; i++)
    {
        if(mapContains(heatPoints, vertices[i]->getNumber()))
        {
            values[i] = heatPoints.find(vertices[i]->getNumber())->second.first;

            for(int j = 0; j < nVertices; j++)
            {
                if(i==j)
                    vertexFactors(i,j) = 1.f;
                else
                    vertexFactors(i,j) = 0.f;
            }
        }
        else if(isBoundary(vertices[i]))
        {
            values[i] = 0.f;

            for(int j = 0; j < nVertices; j++)
            {
                if(i==j)
                    vertexFactors(i,j) = 1.f;
                else
                    vertexFactors(i,j) = 0.f;
            }
        }
        else
        {
            int nNeighbors = 0;
            for(int j = 0; j < nVertices; j++)
            {
                if(areNeighbors(vertices[i], vertices[j]))
                {
                    std::pair<MeshVertex*, MeshVertex*> sharedNeighbors = getSharedNeighbors(vertices[i], vertices[j]);

                    if(sharedNeighbors.first != nullptr && sharedNeighbors.second != nullptr)
                    {
                        vec4 ca = vertices[i]->getAttribute(0) - sharedNeighbors.first->getAttribute(0);
                        vec4 cb = vertices[j]->getAttribute(0) - sharedNeighbors.first->getAttribute(0);
                        vec4 da = vertices[i]->getAttribute(0) - sharedNeighbors.second->getAttribute(0);
                        vec4 db = vertices[j]->getAttribute(0) - sharedNeighbors.second->getAttribute(0);

                        vertexFactors(i,j) = (float) ((dot(ca, cb) + dot(da, db)));
                    } else
                    {
                        vertexFactors(i,j) = 0.f;
                    }
                    nNeighbors++;
                }
                else
                    vertexFactors(i,j) = 0.f;
            }
            vertexFactors(i,i) = -nNeighbors;
            values[i] = 0.f;
        }
    }
    
    // compute X matrix
    // SVD decomposition of a
    Eigen::BDCSVD<Eigen::MatrixXf> svd(vertexFactors, Eigen::ComputeFullU+Eigen::ComputeFullV);

    // solve linear system ax = b
    Eigen::VectorXf x = svd.solve(values);

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
    importer.loadObjFile("Models/laplacianCurved.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(0,1,0.3), 1, 128);
    meshManager->addMesh("laplacianMesh", mesh);
    mesh->setDrawEdges(true);

    // // select boundaries vertices
    map<int, std::pair<float, MeshVertex*>> heatPoints;

    // select heat vertices
    float coords[] = {2,0,0,
              2.5,0,0};

    float coords2[] = {-0.5,0,0,
                        0,0.5,0,
                        0.5,0.5,0,
                        1,0.5,0,
                        1.5,0.5,0,
                        2,0.5,0,
                        2.5,0,0,
                        2.5,-0.5,0,
                        2.5,-1,0,
                        2.5,-1.5,0,
                        2.5,-2,0,
                        2.5,-2.5,0,
                        2.5,-3,0,
                        2.5,-3.5,0,
                        2.5,-4,0,
                        2,-4.5,0,
                        1.5,-4.5,0,
                        1,-4.5,0,
                        0.5,-4.5,0,
                        0,-4.5,0,
                        -0.5,-4,0,
                        -0.5,-3.5,0,
                        0,-3.5,0,
                        0.5,-3.5,0,
                        1,-3.5,0,
                        1.5,-3,0,
                        1.5,-2.5,0,
                        1.5,-2,0,
                        1.5,-1.5,0,
                        1.5,-1,0,
                        1,-0.5,0,
                        0.5,-0.5,0,
                        0,-0.5,0,
                        -0.5,-0.5,0};
    
    for(int i = 0; i < 3*2; i+=3)
    {
        vec3 coord(coords[i], coords[i+1], coords[i+2]);
        vector<MeshVertex*> vertices = mesh->getVertex(coord);
        heatPoints.insert({vertices[0]->getNumber(), {1.f, vertices[0]}});
    }
    
    // for(int i = 0; i < 3*34; i+=3)
    // {
    //     vec3 coord(coords2[i], coords2[i+1], coords2[i+2]);
    //     vector<MeshVertex*> vertices = mesh->getVertex(coord);
    //     heatPoints.insert({vertices[0]->getNumber(), {0.f, vertices[0]}});
    // }

    // compute laplacian
    laplacian(mesh, heatPoints);

    mesh->loadMesh();

    scene.mainLoop();

    return 0;
}