#include "Mesh/MeshVertex.h"
#include "Scene/Scene.h"
#include "Importer/Importer.h"
#include "Scene/MeshManager.h"
#include "Scene/LightManager.h"
#include "Mesh/Mesh.h"
#include "Light/PointLight.h"
#include "Anim/Bone.h"

#include <glm/fwd.hpp>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;


    // --------------------- Mesh Import -------------------

    MeshManager* meshManager = scene.getMeshManager();
    Mesh* mesh = new Mesh("Shaders/skinning.vert", "Shaders/skinning.frag");
    Importer importer2(mesh);
    importer2.loadObjFile("Models/cylinder.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(1,1,1), 0.4, 0.9);
    meshManager->addMesh("cylinder", mesh);


    // --------------------- Bones Creation -------------------

    Bone* boneA = new Bone(vec3(0,-1,0)); // root bone
    Bone* boneB = new Bone(vec3(0,0,0), boneA);


    // --------------------- Weights attribution -------------------

    vector<vec2> verticesWeights;


    // --------------------- Animation with GPU -------------------


    boneB->setRotate(1, rotate(0.9f, vec3(1,0,0)));
    boneA->setRotate(1, rotate(-0.3f, vec3(1,0,0)));


    // --------------------- Lighting and scene loop -------------------

    PointLight p1 = PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9));
    PointLight p2 = PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3));
    PointLight p3 = PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1));
    LightManager* lightManager = scene.getLightManager();
    lightManager->addLight(new PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9)));
    lightManager->addLight(new PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3)));
    lightManager->addLight(new PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1)));
    lightManager->setAmbientLight(vec3(0.2,0.3,0.5), 0.7);

    scene.mainLoop(boneA, boneB, verticesWeights, true);

    return 0;
}