#include "PointLight.h"

vec3 PointLight::getLocation()
{
    return m_location;
}

vec4 PointLight::getColor()
{
    return m_color;
}

void PointLight::setLocation(vec3 location)
{
    m_location = location;
}

void PointLight::setColor(vec4 color)
{
    m_color = color;
}
