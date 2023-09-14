#ifndef LIGHTMANAGER
#define LIGHTMANAGER

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "PointLight.h"

using namespace glm;
using namespace std;

class LightManager
{
    public:
        LightManager() = default;
        ~LightManager() = default;

        void addLight(PointLight* pLight);
        inline vector<vec3> getLocations() {return m_locations;}
        inline vector<vec4> getColors() {return m_colors;}
        
    private:
        vector<vec3> m_locations;
        vector<vec4> m_colors;
};

#endif