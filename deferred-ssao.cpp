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

    BezierManager* bezierManager = scene.getBezierManager();
    MeshManager* meshManager = scene.getMeshManager();
    LightManager* lightManager = scene.getLightManager();
    Renderer* renderer = scene.getRenderer();

    BezierCurve* bezier = new BezierCurve(vec3(0,-5,0), vec3(2, 1, 3), vec3(-2, 2, 2), vec3(0,3,0), renderer);
    bezier->addSegment(vec3(1, 1, 1), vec3(3, 3, 3), vec3(0, 5, 0));
    bezier->compute(16);
    bezier->transform(translate(vec3(10,0,0)));
    bezierManager->addCurve("curve", bezier);

    Material* material = new Material(GGX, vec3(0.5, 0.8, 0.9), vec3(1), 0.7, 0.8);
    Mesh* mesh = new Mesh("Shaders/BRDF.vert", "Shaders/BRDFmicroFacet.frag", material, renderer);
    Importer importer(mesh);
    mesh->transform(translate(vec3(0, 3.1, 0)));
    importer.loadObjFile("Models/bunny.obj");
    meshManager->addMesh("bunny", mesh);

    material = new Material(GGX, vec3(0.7, 0.4, 0.4), vec3(1), .1, 0.2);
    mesh = new Mesh("Shaders/BRDF.vert", "Shaders/BRDFmicroFacet.frag", material, renderer);
    Importer importer2(mesh);
    importer2.loadObjFile("Models/vase.obj");
    meshManager->addMesh("vase", mesh);

    BezierCurve* b0 = new BezierCurve(vec3(-1.5, 0, 0), vec3(-0.5, 0, -1), vec3(0.5, 0, 0), vec3(1.5, 0, 0), renderer);
    BezierCurve* b1 = new BezierCurve(vec3(-1.5, 1, 0), vec3(-0.5, 1, 1), vec3(0.5, 3, 1), vec3(1.5, 1, 0), renderer);
    BezierCurve* b2 = new BezierCurve(vec3(-1.5, 2, 0), vec3(-0.5, 2, 0), vec3(0.5, 5, 2), vec3(1.5, 2, 0), renderer);
    BezierCurve* b3 = new BezierCurve(vec3(-1.5, 3, 0), vec3(-0.5, 3, 0), vec3(0.5, 3, 0), vec3(1.5, 3, 0), renderer);
    BezierSurface* bezierS = new BezierSurface(b0, b1, b2, b3, renderer);
    bezierS->compute(64, 64, "Shaders/BRDF.vert", "Shaders/BRDF.frag");
    bezierS->transform(translate(vec3(5,0,0)));
    bezierManager->addSurface("surface1", bezierS);

    b0 = new BezierCurve(vec3(-1.5, 0, 0), vec3(-0.5, 0, 0), vec3(0.5, 0, 0), vec3(1.5, 0, 0), renderer);
    b1 = new BezierCurve(vec3(-1.5, 1, 0), vec3(-0.5, 1, 0), vec3(0.5, 1, 1), vec3(1.5, 1, 0), renderer);
    b2 = new BezierCurve(vec3(-1.5, 2, 0), vec3(-0.5, 2, 0), vec3(0.5, 2, 2), vec3(1.5, 2, 0), renderer);
    b3 = new BezierCurve(vec3(-1.5, 3, 0), vec3(-0.5, 3, 0), vec3(0.5, 3, 0), vec3(1.5, 3, 0), renderer);
    bezierS = new BezierSurface(b0, b1, b2, b3, renderer);
    bezierS->compute(64, 64, "Shaders/BRDF.vert", "Shaders/BRDF.frag");
    bezierS->transform(translate(vec3(-5,0,0)));
    bezierS->getMesh()->getMaterial()->loadDiffuseColorTexture("Shaders/metal_plate_diff.jpg");
    bezierS->getMesh()->getMaterial()->loadShininessTexture("Shaders/metal_plate_rough.jpg");
    bezierS->getMesh()->getMaterial()->loadSpecularColorTexture("Shaders/metal_plate_spec.jpg");
    bezierManager->addSurface("surface2", bezierS);

    PointLight p1 = PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9));
    PointLight p2 = PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3));
    PointLight p3 = PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1));
    lightManager->addLight(new PointLight(vec4(4,10,4,1), vec3(1,0.95,0.9)));
    lightManager->addLight(new PointLight(vec4(-4,5,4,1), vec3(1,0.6,0.3)));
    lightManager->addLight(new PointLight(vec4(0,5,-3,1), vec3(0.6,0.9,1)));
    lightManager->setAmbientLight(vec3(0.2,0.3,0.5), 0.7);


    scene.mainLoop();

    return 0;
}