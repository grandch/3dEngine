#include "Scene/Scene.h"
#include "Importer/Importer.h"
#include "Scene/MeshManager.h"
#include "Scene/LightManager.h"
#include "Scene/BezierManager.h"
#include "Mesh/Mesh.h"
#include "Light/PointLight.h"
#include "Bezier/BezierCurve.h"
#include "Bezier/BezierSurface.h"
#include "Renderer/Renderer.h"
#include "Shaders/Material.h"

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    MeshManager* meshManager = scene.getMeshManager();
    LightManager* lightManager = scene.getLightManager();
    Renderer* renderer = scene.getRenderer();

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            Material* material = new Material(GGX, vec3(0.9, 0.1, 0.2), vec3(1), 0.1 * i, 0.1 * j);
            Mesh* mesh = new Mesh("Shaders/BRDF.vert", "Shaders/BRDFmicroFacet.frag", material, renderer);
            Importer importer(mesh);
            mesh->transform(translate(vec3(2 * i - 10, 2 * j - 10, 0)));
            importer.loadObjFile("Models/sphere.obj");
            meshManager->addMesh("sphere", mesh);
        }
    }


    PointLight p1 = PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9));
    PointLight p2 = PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3));
    PointLight p3 = PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1));
    lightManager->addLight(new PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9)));
    lightManager->addLight(new PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3)));
    lightManager->addLight(new PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1)));
    lightManager->setAmbientLight(vec3(0.2,0.2,0.3), 1.);


    scene.mainLoop();

    return 0;
}