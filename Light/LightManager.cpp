#include "LightManager.h"
#include <iostream>

LightManager::LightManager() : m_nbLight(0)
{}

LightManager::~LightManager()
{
}

void LightManager::addLight(PointLight *pLight)
{
    if(m_nbLight < 10)
    {
        vec4 location = pLight->getLocation();
        m_locations[m_nbLight*4] = location[0]; 
        m_locations[m_nbLight*4+1] = location[1]; 
        m_locations[m_nbLight*4+2] = location[2]; 
        m_locations[m_nbLight*4+3] = location[3]; 

        vec3 color = pLight->getColor();
        m_colors[m_nbLight*3] = color[0];
        m_colors[m_nbLight*3+1] = color[1];
        m_colors[m_nbLight*3+2] = color[2];

        m_nbLight++;
    }
}

GLfloat *LightManager::getLocations()
{
    return &(m_locations[0]);
}

GLfloat *LightManager::getColors()
{
    return &(m_colors[0]);
}

int LightManager::getNbLights()
{
    return m_nbLight;
}

void LightManager::setAmbientLight(vec3 color, float strength)
{
    m_ambientColor = color;
    m_ambientStrength = strength;
}

vec3 LightManager::getAmbientColor()
{
    return m_ambientColor;
}

float LightManager::getAmbientStrength()
{
    return m_ambientStrength;
}
