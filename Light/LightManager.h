#ifndef LIGHTMANAGER
#define LIGHTMANAGER

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "PointLight.h"
#include "../Shaders/Shader.h"

using namespace glm;
using namespace std;

class LightManager
{
    public:
        LightManager();
        ~LightManager();

        void addLight(PointLight* pLight);
        GLfloat* getLocations();
        GLfloat* getColors();
        int getNbLights();

        void setAmbientLight(vec3 color, float strength);
        vec3 getAmbientColor();
        float getAmbientStrength();

        void sendDataToShader(Shader* shader);
        
    private:
        GLfloat m_locations[40];
        GLfloat m_colors[30];
        int m_nbLight;

        vec3 m_ambientColor;
        float m_ambientStrength;
};

#endif