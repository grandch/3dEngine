#include "Scene.h"

using namespace glm;

Scene::Scene(string title, int width, int height): m_windowTitle(title), m_wWidth(width), m_wHeight(height), m_window(0), m_openGLContext(0)
{}

Scene::~Scene()
{
    SDL_GL_DeleteContext(m_openGLContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}



bool Scene::initWindow()
{
    int depthSize(24);
    //SDL init

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Error while loading the SDL : " << SDL_GetError() << endl;
        SDL_Quit();

        return false;
    }

    //openGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    //doublebuffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depthSize);


    //window init

    m_window = SDL_CreateWindow("SDLxOPENGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_wWidth, m_wHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (m_window == 0)
    {
        cout << "Error while loading the window : " << SDL_GetError() << endl;
        SDL_Quit();

        return false;
    }

    //openGLcontext init

    m_openGLContext = SDL_GL_CreateContext(m_window);

    if (m_openGLContext == 0)
    {
        cout << "Error while loading OpenGL context : " << SDL_GetError() << endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        return false;
    }

    return true;
}


bool Scene::initGL()
{
    //init glew (windows) TO DO

    glEnable(GL_DEPTH_TEST); //depth buffer activation

    return true;
}


void Scene::mainLoop()
{
    bool ending(false);
    unsigned int frameRate(1000/50);
    Uint32 loopBeg(0), loopEnd(0), time(0);

    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_wWidth / m_wHeight, 1.0, 100.0); //init the camera
    modelview = mat4(0.1);

    Cube cube(2.0, "Shaders/couleur3D.vert", "Shaders/couleur3D.frag");

    float angle(0.0);

    while (!ending)
    {
        loopBeg = SDL_GetTicks();

        SDL_PollEvent(&m_events);

        if(m_events.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            ending = true;
        }

        angle += 0.01;

        if(angle >= 360.0)
        {
            angle -= 360.0;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the window and the depth buffer
        modelview = lookAt(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0)); //init the camera

        modelview = rotate(modelview, angle, vec3(0, 1, 0));

        cube.draw(projection, modelview);

        SDL_GL_SwapWindow(m_window); //refresh the window

        loopEnd = SDL_GetTicks();
        time = loopBeg - loopEnd;

        if(time < frameRate)
        {
            SDL_Delay(frameRate - time);
        }
    }
}


bool Scene::initScene()
{
    if (this->initWindow() == false)
    return false;

    if (this->initGL() == false)
    return false;

    return true;
}