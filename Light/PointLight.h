#ifndef POINTLIGHT
#define POINTLIGHT

#include "../Shaders/Shader.h"
#include "../Scene/MeshManager.h"

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class PointLight 
{
    public:
        PointLight(vec4 location = {0,0,0,1}, vec3 color = {0.7,0.7,0.7});
        ~PointLight() = default;

        vec4 getLocation(); 
        vec3 getColor(); 

        void setLocation(vec4 location); 
        void setColor(vec3 color);

        void shadowMap(MeshManager* meshes);

    private:
        vec4 m_location;
        vec3 m_color;

        unsigned int m_depthMapFBO;
        unsigned int m_depthMap;

        mat4 m_lightSpaceMatrix;

        Shader m_shader;
};
#endif