#ifndef BEZIERMANAGER
#define BEZIERMANAGER
#define GLM_ENABLE_EXPERIMENTAL

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Bezier/BezierCurve.h"
#include "../Bezier/BezierSurface.h"
#include "LightManager.h"

class BezierManager
{
    public:
        BezierManager();
        ~BezierManager();
    
        void addCurve(string name, BezierCurve* curve);
        BezierCurve* getCurve(string name);
        void removeCurve(string name);
        void transformCurve(string name, mat4 transform);

        void addSurface(string name, BezierSurface* surface);
        BezierSurface* getSurface(string name);
        void removeSurface(string name);
        void transformSurface(string name, mat4 transform);

        void draw(mat4 &projection, mat4 &view, LightManager* lightManager);

    private:
        map<string, BezierCurve*> m_curves;
        map<string, BezierSurface*> m_surfaces;
};

#endif