#include "Scene/Scene.h"
#include "Importer/Importer.h"
#include "Scene/MeshManager.h"
#include "Scene/LightManager.h"
#include "Mesh/Mesh.h"
#include "Light/PointLight.h"

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;

    MeshManager* meshManager = scene.getMeshManager();
    LightManager* lightManager = scene.getLightManager();

    Mesh* mesh = new Mesh("Shaders/BRDF.vert", "Shaders/BRDFmicroFacet.frag");
    Importer importer2(mesh);
    importer2.loadObjFile("Models/vase.obj");
    mesh->setMaterial(vec3(1,1,1), vec3(1,1,1), 0.4, 0.5);
    meshManager->addMesh("vase", mesh);

    PointLight p1 = PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9));
    PointLight p2 = PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3));
    PointLight p3 = PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1));
    lightManager->addLight(new PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9)));
    lightManager->addLight(new PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3)));
    lightManager->addLight(new PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1)));
    lightManager->setAmbientLight(vec3(0.2,0.3,0.5), 0.3);


    scene.mainLoop();

    return 0;
}