#ifndef POINTLIGHT
#define POINTLIGHT

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class PointLight 
{
    public:
        PointLight(vec3 location = {0,0,0}, vec4 color = {0.7,0.7,0.7,1}) : m_location(location), m_color(color) {}
        ~PointLight() = default;

        vec3 getLocation(); 
        vec4 getColor(); 

        void setLocation(vec3 location); 
        void setColor(vec4 color);

    private:
        vec3 m_location;
        vec4 m_color;
};
#endif