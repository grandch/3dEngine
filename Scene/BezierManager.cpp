#include "BezierManager.h"

BezierManager::BezierManager()
{}

BezierManager::~BezierManager()
{}

void BezierManager::addCurve(string name, BezierCurve *curve)
{
    m_curves.insert({name, curve});
}

BezierCurve *BezierManager::getCurve(string name)
{
    return m_curves[name];
}

void BezierManager::removeCurve(string name)
{
    m_curves.erase(name);
}

void BezierManager::transformCurve(string name, mat4 transform)
{
    m_curves[name]->transform(transform);
}

void BezierManager::addSurface(string name, BezierSurface *surface)
{
    m_surfaces.insert({name, surface});
}

BezierSurface *BezierManager::getSurface(string name)
{
    return m_surfaces[name];
}

void BezierManager::removeSurface(string name)
{
    m_surfaces.erase(name);
}

void BezierManager::transformSurface(string name, mat4 transform)
{
    m_surfaces[name]->transform(transform);
}

void BezierManager::draw(mat4 &projection, mat4 &view, LightManager *lightManager)
{
    for(auto i = m_curves.begin(); i != m_curves.end(); ++i)
    {
        i->second->draw(projection, view);
    }

    for(auto i = m_surfaces.begin(); i != m_surfaces.end(); ++i)
    {
        i->second->draw(projection, view, lightManager);
    }
}
