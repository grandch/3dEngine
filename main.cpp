#include "Scene/Scene.h"

int main(int argc, char **argv)
{
    Scene scene("3dEngine", 1280, 1024);

    if (scene.initScene() == false)
        return -1;

    scene.mainLoop();

    return 0;
}