#include "PointLight.h"

vec4 PointLight::getLocation()
{
    return m_location;
}

vec3 PointLight::getColor()
{
    return m_color;
}

void PointLight::setLocation(vec4 location)
{
    m_location = location;
}

void PointLight::setColor(vec3 color)
{
    m_color = color;
}
