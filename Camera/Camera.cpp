#include "Camera.h"

Camera::Camera() : m_phi(0.0), m_theta(0.0), m_orientation(), m_verticalAxis(0, 0, 1), m_lateralAxis(), m_position(), m_target()
{}

Camera::Camera(vec3 position, vec3 target, vec3 verticalAxis) : m_phi(-35.26), m_theta(-135), m_orientation(), m_verticalAxis(verticalAxis), m_lateralAxis(), m_position(position), m_target(target)
{}

Camera::~Camera()
{}


void Camera::orientate(int xRel, int yRel)
{
    m_phi += -yRel * 0.5;
    m_theta += -xRel * 0.5;

    if(m_phi > 89.0)
    {
        m_phi = 89.0;
    }

    if(m_phi < -89.0)
    {
        m_phi = -89.0;
    }

    float phiRadian = m_phi * M_PI / 180;
    float thetaRadian = m_theta * M_PI / 180;

    if(m_verticalAxis.x == 1.0)
    {
        m_orientation.x = sin(phiRadian);
        m_orientation.y = cos(phiRadian) * cos(thetaRadian);
        m_orientation.z = cos(phiRadian) * sin(thetaRadian);
    }
    else if (m_verticalAxis.y == 1.0) 
    {
        m_orientation.x = cos(phiRadian) * sin(thetaRadian);
        m_orientation.y = sin(phiRadian);
        m_orientation.z = cos(phiRadian) * cos(thetaRadian);
    }
    else
    {
        m_orientation.x = cos(phiRadian) * cos(thetaRadian);
        m_orientation.y = cos(phiRadian) * sin(thetaRadian);
        m_orientation.z = sin(phiRadian);
    }

    m_lateralAxis = normalize(cross(m_verticalAxis, m_orientation));

    m_target = m_position + m_orientation;
}

void Camera::move(Input const &input)
{
    if(input.getMouseMovement() && input.getMouseButton(2))
    {
        orientate(input.getXRel(), input.getYRel());
    }

    if(input.getKey(SDL_SCANCODE_UP) || input.getKey(SDL_SCANCODE_W))
    {
        m_position += m_orientation * 0.3f;
        m_target = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_DOWN) || input.getKey(SDL_SCANCODE_S))
    {
        m_position -= m_orientation * 0.3f;
        m_target = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_LEFT) || input.getKey(SDL_SCANCODE_A))
    {
        m_position += m_lateralAxis * 0.3f;
        m_target = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_RIGHT) || input.getKey(SDL_SCANCODE_D))
    {
        m_position -= m_lateralAxis * 0.3f;
        m_target = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_Q))
    {
        m_position -= m_verticalAxis * 0.3f;
        m_target = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_E))
    {
        m_position += m_verticalAxis * 0.3f;
        m_target = m_position + m_orientation;
    }
}

void Camera::lookAt(mat4 &view)
{
    view = glm::lookAt(m_position, m_target, m_verticalAxis);
}