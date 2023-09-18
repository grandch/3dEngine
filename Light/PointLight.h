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
        PointLight(vec4 location = {0,0,0,1}, vec3 color = {0.7,0.7,0.7}) : m_location(location), m_color(color) {}
        ~PointLight() = default;

        vec4 getLocation(); 
        vec3 getColor(); 

        void setLocation(vec4 location); 
        void setColor(vec3 color);

    private:
        vec4 m_location;
        vec3 m_color;
};
#endif