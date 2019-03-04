#include "Scene.h"

int main(int argc, char **argv)
{
    Scene scene("SDLxGL", 800, 600);

    if (scene.initScene() == false)
    return -1;

    scene.mainLoop();

    return 0;
}