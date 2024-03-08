#include "Scene.h"

Scene::Scene(string title, int width, int height): m_windowTitle(title), m_wWidth(width), m_wHeight(height), m_window(0), m_openGLContext(0), m_input()
{
    initWindow();
    initGL();
    m_lightManager = new LightManager(&m_view);
    m_meshManager = new MeshManager();
    m_bezierManager = new BezierManager();
    m_renderer = new Renderer(width, height, m_lightManager);
    m_axis = new Axis(m_renderer);
}

Scene::~Scene()
{
    free(m_axis);
    free(m_lightManager);
    free(m_meshManager);
    free(m_bezierManager);
    SDL_GL_DeleteContext(m_openGLContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

LightManager *Scene::getLightManager()
{
    return m_lightManager;
}

MeshManager *Scene::getMeshManager()
{
    return m_meshManager;
}

BezierManager *Scene::getBezierManager()
{
    return m_bezierManager;
}

Renderer *Scene::getRenderer()
{
    return m_renderer;
}

bool Scene::initWindow()
{
    int depthSize = 24;

    //SDL init
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Error while loading the SDL : " << SDL_GetError() << endl;
        SDL_Quit();

        return false;
    }

    //openGL version (3.3 core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //doublebuffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depthSize);


    //window init

    m_window = SDL_CreateWindow("3dEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_wWidth, m_wHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (m_window == 0)
    {
        cout << "Failed to load the window : " << SDL_GetError() << endl;
        SDL_Quit();

        return false;
    }

    //openGLcontext init

    m_openGLContext = SDL_GL_CreateContext(m_window);

    if (m_openGLContext == 0)
    {
        cout << "Failed to load OpenGL context : " << SDL_GetError() << endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        return false;
    }

    return true;
}

bool Scene::initGL()
{
    glEnable(GL_DEPTH_TEST); //depth buffer activation

    return true;
}

void Scene::mainLoop()
{
    unsigned int frameRate(1000/50);
    Uint32 loopBeg(0), loopEnd(0), time(0);

    mat4 projection;

    projection = perspective(70.0, (double) m_wWidth / m_wHeight, 1.0, 100.0); //init the camera
    m_view = mat4(0.1);

    CameraRotateAround camera(vec3(-15, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
    camera.lookAt(m_view);
    m_input.showCursor(false);
    m_input.cursorCapture(true);

    float angleY(0.0);

    while (!m_input.end())
    {
        loopBeg = SDL_GetTicks();

        m_input.updateEvent();

        if(m_input.getKey(SDL_SCANCODE_ESCAPE))
        {
            break;
        }

        if(m_input.getKey(SDL_SCANCODE_RETURN))
        {
            screenshot();
        }

        if(m_input.getKey(SDL_SCANCODE_C))
        {
            // m_mesh->compileShaders();
            // m_bezierS->getMesh()->compileShaders();
        }

        camera.move(m_input);

        if(m_input.getKey(SDL_SCANCODE_LEFT))
            angleY -= 0.01;
        if(m_input.getKey(SDL_SCANCODE_RIGHT))
            angleY += 0.01;

        if(angleY >= 360.0)
        {
            angleY -= 360.0;
        }
        else if (angleY <= -360.0)
        {
            angleY += 360.0;
        }
        
        glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the window and the depth buffer
        camera.lookAt(m_view);

        // m_axis->draw(projection, m_view);
        // m_meshManager->draw(projection, m_view, m_lightManager);
        // m_bezierManager->draw(projection, m_view, m_lightManager);

        m_renderer->render(projection, m_view);

        SDL_GL_SwapWindow(m_window); //refresh the window

        loopEnd = SDL_GetTicks();
        time = loopBeg - loopEnd;

        if(time < frameRate)
        {
            SDL_Delay(frameRate - time);
        }
    }
}

void Scene::screenshot()
{
    string filename = "screenshot.tga";

    const int numberOfPixels = m_wWidth * m_wHeight * 3;
    unsigned char pixels[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, m_wWidth, m_wHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    FILE *outputFile = fopen(filename.c_str(), "w");
    short header[] = {0, 2, 0, 0, 0, 0, (short) m_wWidth, (short) m_wHeight, 24};

    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(pixels, numberOfPixels, 1, outputFile);
    fclose(outputFile);

    system("sh convert.sh");

    printf("Finish writing to file.\n");
}