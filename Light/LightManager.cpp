#include "LightManager.h"

void LightManager::addLight(PointLight *pLight)
{
    m_locations.push_back(pLight->getLocation()); 
    m_colors.push_back(pLight->getColor());
}