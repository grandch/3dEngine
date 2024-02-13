#include "CameraRotateAround.h"
#include <iostream>

CameraRotateAround::CameraRotateAround()
{}

CameraRotateAround::CameraRotateAround(vec3 position, vec3 target, vec3 up)
{
    m_position = position;
    m_target = target;
    m_up = up;

    m_orientation = normalize(m_target - m_position);
    m_lateralAxis = normalize(cross(m_up, m_orientation));
}

CameraRotateAround::~CameraRotateAround()
{}

void CameraRotateAround::orientate(int xRel, int yRel)
{
    m_position = vec3(vec4(m_position, 1) * 0.1f * rotate((float)yRel, m_up));
    m_orientation = normalize(m_target - m_position);
    m_lateralAxis = normalize(cross(m_up, m_orientation));
    m_position = vec3(vec4(m_position, 1) * 0.1f * rotate((float)xRel, m_lateralAxis));
    m_orientation = normalize(m_target - m_position);
}

void CameraRotateAround::translate(int xRel, int yRel)
{
    // add to position and target xRel * lateral vector and yRel * up
    m_position += 0.3f * (float)xRel * m_lateralAxis;
    m_position += 0.3f * (float)yRel * m_up;

    m_target += 0.3f * (float)xRel * m_lateralAxis;
    m_target += 0.3f * (float)yRel * m_up;
}

void CameraRotateAround::zoom(int y)
{
    m_position += y * 0.03f * m_orientation;
}

void CameraRotateAround::move(Input const &input)
{
    if(input.getMouseMovement() && input.getMouseButton(2) && (input.getKey(SDL_SCANCODE_LSHIFT) || input.getKey(SDL_SCANCODE_RSHIFT)))
    {
        translate(input.getXRel(), input.getYRel());
    }
    else if(input.getMouseMovement() && input.getMouseButton(2))
    {
        orientate(input.getXRel(), input.getYRel());
    }
    
    if(input.getWheelMovement())
    {
        zoom(input.getMouseWheelY());
    }
}
