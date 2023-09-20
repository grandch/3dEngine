#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "MeshManager.h"
#include "LightManager.h"
#include "BezierManager.h"
#include "Axis/Axis.h"

class SceneManager
{
    public:
        SceneManager();
        ~SceneManager();
    private:
        MeshManager* m_meshManager;
        BezierManager* m_bezierManager;
        LightManager* m_lightManager;
};

#endif