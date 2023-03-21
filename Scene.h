#ifndef DEF_SCENE
#define DEF_SCENE
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Bezier/BezierCurve.h"
#include "Bezier/BezierSurface.h"
#include "Mesh/Mesh.h"
#include "Shaders/Shader.h"
#include "Inputs/Input.h"
#include "Camera/Camera.h"
#include "Importer/Importer.h"
#include "Axis/Axis.h"

#include <GLES3/gl3.h>

using namespace std;
using namespace glm;

class Scene
{
    public:

        Scene(string title, int width, int height);
        ~Scene();

        void initModel(string file);
        bool initWindow();
        bool initGL();

        void mainLoop();

        bool initScene();

        void screenshot();

    private:

        string m_windowTitle;
        int m_wWidth;
        int m_wHeight;

        SDL_Window* m_window;
        SDL_GLContext m_openGLContext;
        SDL_Event m_events;
        Input m_input;

        Mesh* m_mesh;
        BezierCurve* m_bezier;
        BezierSurface* m_bezierS;
        Axis* m_axis;
};
#endif