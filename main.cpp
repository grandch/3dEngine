#include <iostream>

#define GL3_PROTOTYPES 1

#include "Scene.h"

using namespace std;

int main(int argc, char **argv)
{
    Scene scene("SDLxGL", 800, 600);

    if (scene.initScene() == false)
    return -1;

    scene.mainLoop();

    return 0;
}