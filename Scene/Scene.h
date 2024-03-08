#ifndef DEF_SCENE
#define DEF_SCENE
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Bezier/BezierCurve.h"
#include "../Bezier/BezierSurface.h"
#include "../Mesh/Mesh.h"
#include "../Shaders/Shader.h"
#include "../Inputs/Input.h"
#include "../Camera/CameraRotateAround.h"
#include "../Importer/Importer.h"
#include "Axis/Axis.h"
#include "../Light/PointLight.h"
#include "LightManager.h"
#include "MeshManager.h"
#include "BezierManager.h"
#include "../Renderer/Renderer.h"

#include <GLES3/gl3.h>

using namespace std;
using namespace glm;

class Scene
{
    public:

        Scene(string title, int width, int height);
        ~Scene();

        LightManager* getLightManager();
        MeshManager* getMeshManager();
        BezierManager* getBezierManager();
        Renderer* getRenderer();

        bool initWindow();
        bool initGL();

        void mainLoop();

        void screenshot();

    private:

        mat4 m_view;

        string m_windowTitle;
        int m_wWidth;
        int m_wHeight;

        SDL_Window* m_window;
        SDL_GLContext m_openGLContext;
        SDL_Event m_events;
        Input m_input;

        Axis* m_axis;

        LightManager* m_lightManager;
        MeshManager* m_meshManager;
        BezierManager* m_bezierManager;

        Renderer* m_renderer;
};
#endif