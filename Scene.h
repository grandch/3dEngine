#ifndef DEF_SCENE
#define DEF_SCENE
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"
#include "Shader.h"

// Include Windows
#ifdef WIN32
#include <GL/glew.h>


// Include Mac
#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include UNIX/Linux
#else

#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>

#endif


using namespace std;

class Scene
{
    public:

        Scene(string title, int width, int height);
        ~Scene();

        bool initWindow();
        bool initGL();
        void mainLoop();
        bool initScene();

    private:

        string m_windowTitle;
        int m_wWidth;
        int m_wHeight;

        SDL_Window * m_window;
        SDL_GLContext m_openGLContext;
        SDL_Event m_events;
};
#endif