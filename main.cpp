#include "Scene.h"

int main(int argc, char **argv)
{
    Scene scene("SDLxGL", 1280, 1024);

    if (scene.initScene() == false)
    return -1;

    scene.mainLoop();

    return 0;
}